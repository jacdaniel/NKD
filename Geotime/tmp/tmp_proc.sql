create or replace PROCEDURE "QUERY_MASTER_DATA_PROC" (
REQUEST_ID IN NUMBER,
cur_OUT OUT SYS_REFCURSOR)
is
BEGIN

 DECLARE
 --   REQUEST_ID  NUMBER:=392;
    FINAL_REQUEST   VARCHAR2(4000) :='SELECT * FROM MASTERDATA MD WHERE MD.PROMOTE_STATUS = ''MASTER'' AND MD.ID IN ((' ;
    IS_SHAPE_ENABLE NUMBER;
    SHAPE MDSYS.SDO_GEOMETRY;
    CONDITION REQUEST_LINE.CONDITION%TYPE;
    VALUE_LONG REQUEST_LINE.VALUE_LONG%TYPE;
    VALUE_DOUBLE REQUEST_LINE.VALUE_DOUBLE%TYPE;
    VALUE_DATE REQUEST_LINE.VALUE_DATE%TYPE;
    VALUE_STRING REQUEST_LINE.VALUE_STRING%TYPE;
    FIELD_CLASS REQUEST_LINE.FIELD_CLASS%TYPE;
    --VALUE_CLOB REQUEST_LINE.VALUE_CLOB%TYPE;
    IS_OPEN_BLOCK NUMBER;
    IS_CLOSE_BLOCK NUMBER;
    IS_AND NUMBER;
    IS_OR NUMBER;
    LINE_COUNT NUMBER :=0;
    KEY_METADATA_FIELD REQUEST_LINE.KEY_METADATA_FIELD%TYPE;
    FAMILY  REQUEST_LINE.FAMILY%TYPE;

    CURSOR CURSOR_REQUEST_LINE IS
    SELECT KEY_METADATA_FIELD, CONDITION, FIELD_CLASS, VALUE_LONG, VALUE_DOUBLE ,VALUE_DATE, VALUE_STRING, IS_OPEN_BLOCK, IS_CLOSE_BLOCK,IS_AND,IS_OR,FAMILY
    FROM REQUEST_LINE WHERE
    ID_REQUEST = REQUEST_ID
    ORDER BY REQUEST_LINE_NB;

    BEGIN
    
    DBMS_OUTPUT.ENABLE( 1000000 ) ;-- Ouverture du tampon --
    select R.IS_SHAPE_ENABLED, R.SHAPE  INTO IS_SHAPE_ENABLE, SHAPE from REQUEST R where id = REQUEST_ID;
    DBMS_OUTPUT.PUT_LINE('IS SHAPE ENABLE : ' || IS_SHAPE_ENABLE) ;
    OPEN CURSOR_REQUEST_LINE;
    
    LOOP
    FETCH CURSOR_REQUEST_LINE INTO  KEY_METADATA_FIELD, CONDITION, FIELD_CLASS, VALUE_LONG, VALUE_DOUBLE ,VALUE_DATE, VALUE_STRING, IS_OPEN_BLOCK, IS_CLOSE_BLOCK,IS_AND,IS_OR,FAMILY ;
        EXIT WHEN CURSOR_REQUEST_LINE%NOTFOUND;
          DBMS_OUTPUT.PUT_LINE('REQ LINE : ' || KEY_METADATA_FIELD || ' | ' ||CONDITION || ' | ' || FIELD_CLASS || ' | ' ||  VALUE_LONG || ' | ' || VALUE_DOUBLE || ' | ' ||VALUE_DATE|| ' | ' || VALUE_STRING|| ' | ' ||
           IS_OPEN_BLOCK|| ' | ' || IS_CLOSE_BLOCK|| ' | ' ||IS_AND|| ' | ' ||IS_OR|| ' | ' ||FAMILY ) ;

          -- Ouverture de block
          IF( IS_OPEN_BLOCK = 1 ) THEN
            FINAL_REQUEST := FINAL_REQUEST || ' ( ' ;
           -- IS_CLOSE_BLOCK
          ELSIF ( IS_CLOSE_BLOCK = 1 ) THEN
            FINAL_REQUEST := FINAL_REQUEST || ' ) ' ;
          -- IS_AND
          ELSIF ( IS_AND = 1 ) THEN
             FINAL_REQUEST := FINAL_REQUEST || ' intersect ' ;
          -- IS_OR
          ELSIF ( IS_OR = 1 ) THEN
             FINAL_REQUEST := FINAL_REQUEST || ' union ' ;
         --  CRITERE CHAMPS
          ELSIF (( FAMILY = 'GEODESY') OR ( FAMILY = 'GEOMETRY_SET') OR ( FAMILY = 'GEOMETRY') OR
                 ( FAMILY = 'DATASET') OR ( FAMILY = 'AUTOMATIC') OR ( FAMILY = 'MANUAL_MANDATORY')  OR ( FAMILY = 'MANUAL_OPTIONAL') ) THEN
                IF (( FAMILY = 'GEODESY') OR ( FAMILY = 'GEOMETRY_SET') OR ( FAMILY = 'GEOMETRY') ) THEN
                -- CAS CRITERES GEOMETRIQUES
                 FINAL_REQUEST := FINAL_REQUEST || ' (select id_masterdata from GEO_MAST_META_VIEW where KEY_METADATA_FIELD = ''' ;
                ELSE
                -- CAS CRITERES SUR MASTER DATA
                 FINAL_REQUEST := FINAL_REQUEST || ' (select id_masterdata from METADATA where KEY_METADATA_FIELD = ''' ;
                END IF;
            --NOM CHAMPS
             FINAL_REQUEST := FINAL_REQUEST || KEY_METADATA_FIELD ;
             -- FAMILLE (Juste pour une fin d'amelioration des performances)
             FINAL_REQUEST := FINAL_REQUEST || ''' AND FAMILY =''' ;
             FINAL_REQUEST := FINAL_REQUEST || FAMILY ;
             FINAL_REQUEST := FINAL_REQUEST || ''' AND ' ;

             -- cas du different : prise en compte des null value
             VALUE_STRING := TRIM(VALUE_STRING);
             IF (CONDITION = 'DIFFERENT' AND FIELD_CLASS = 'STRING' AND VALUE_STRING IS NOT NULL) THEN
                FINAL_REQUEST := FINAL_REQUEST || ' ( ';
             END IF;

             -- EN FONCTION DU TYPE
                IF (FIELD_CLASS = 'STRING' OR FIELD_CLASS = 'ENUM') THEN
                  IF (CONDITION = 'EQUALS') THEN
                    FINAL_REQUEST := FINAL_REQUEST || 'VALUE_STRING';
                  ELSE
                    FINAL_REQUEST := FINAL_REQUEST || 'UPPER(VALUE_STRING)';
                  END IF;
                ELSIF (FIELD_CLASS =  'CLOB' ) THEN
                  FINAL_REQUEST := FINAL_REQUEST || 'VALUE_CLOB';
                ELSIF (FIELD_CLASS =  'LONG' ) THEN
                  FINAL_REQUEST := FINAL_REQUEST || 'VALUE_LONG';
                ELSIF (FIELD_CLASS = 'DOUBLE' ) THEN
                  FINAL_REQUEST := FINAL_REQUEST || 'VALUE_DOUBLE';
                ELSIF (FIELD_CLASS = 'DATE' ) THEN
                  FINAL_REQUEST := FINAL_REQUEST || 'TO_DATE(VALUE_DATE,''dd-mm-yyyy'')';
                END IF;
            -- OPERATION DE COMPARAISON
	        -- Cas particulier pour les chaines vides
               IF ((FIELD_CLASS = 'STRING' OR FIELD_CLASS = 'ENUM' OR FIELD_CLASS =  'CLOB') AND VALUE_STRING IS NULL ) THEN
                 IF (CONDITION = 'EQUALS') THEN
                  FINAL_REQUEST := FINAL_REQUEST || ' IS NULL';
                 ELSE
                  FINAL_REQUEST := FINAL_REQUEST || ' IS NOT NULL';
		 END IF; -- CONDITION = 'EQUALS'
	      ELSE
                IF (CONDITION = 'EQUALS') THEN
                  FINAL_REQUEST := FINAL_REQUEST || ' = ';
                ELSIF (CONDITION =  'DIFFERENT' ) THEN
                  FINAL_REQUEST := FINAL_REQUEST || ' <> ';
                ELSIF (CONDITION = 'SUPERIOR' ) THEN
                  FINAL_REQUEST := FINAL_REQUEST || ' > ';
                ELSIF (CONDITION = 'INFERIEUR' ) THEN
                  FINAL_REQUEST := FINAL_REQUEST || ' < ';
                ELSIF (CONDITION = 'CONTAINS' OR CONDITION = 'START WITH' OR CONDITION = 'END WITH' ) THEN
                  FINAL_REQUEST := FINAL_REQUEST || ' LIKE ';
                END IF;
                -- CAS des chaines
                IF (FIELD_CLASS = 'STRING' OR FIELD_CLASS = 'ENUM' OR FIELD_CLASS =  'CLOB') THEN
                   --ouverture cote
                   IF (CONDITION = 'CONTAINS' OR CONDITION = 'END WITH' ) THEN
                        FINAL_REQUEST := FINAL_REQUEST || 'UPPER(''%';
                   ELSIF (CONDITION = 'EQUALS') THEN
                        FINAL_REQUEST := FINAL_REQUEST || '''';
                   ELSE
                        FINAL_REQUEST := FINAL_REQUEST || 'UPPER(''';
                   END IF;
                   -- valeur a comparer
                   IF (FIELD_CLASS = 'STRING' OR  FIELD_CLASS = 'ENUM' OR  FIELD_CLASS = 'CLOB') THEN
                   		-- we need to escape the potential underscore in the search value because it is a SQL wildcard
                   		IF (CONDITION = 'CONTAINS' OR CONDITION = 'END WITH' OR CONDITION = 'START WITH') THEN
                   			FINAL_REQUEST := FINAL_REQUEST ||  REPLACE(VALUE_STRING, '_', '\_');
                   		ELSE
                            FINAL_REQUEST := FINAL_REQUEST ||  VALUE_STRING ;
                        END IF;
                   END IF;
                   --fermeture cote
                   IF (CONDITION = 'CONTAINS' OR CONDITION = 'START WITH' ) THEN
                        FINAL_REQUEST := FINAL_REQUEST || '%'')';
                   ELSIF (CONDITION = 'EQUALS') THEN
                        FINAL_REQUEST := FINAL_REQUEST || '''';
                   ELSE
                        FINAL_REQUEST := FINAL_REQUEST || ''')';
                   END IF; -- CAS des chaines

                   IF (CONDITION = 'CONTAINS' OR CONDITION = 'END WITH' OR CONDITION = 'START WITH') THEN
                        FINAL_REQUEST := FINAL_REQUEST || ' escape''\'';
                    END IF;

                   -- cas du different : prise en compte des null value
                   IF (CONDITION = 'DIFFERENT' AND FIELD_CLASS = 'STRING') THEN
                       FINAL_REQUEST := FINAL_REQUEST || ' OR VALUE_STRING IS NULL )';
                   END IF;
               -- AUTRES CAS
               ELSE
               -- valeur a comparer
                   IF (FIELD_CLASS = 'LONG') THEN
                        FINAL_REQUEST := FINAL_REQUEST ||  VALUE_LONG ;
                   ELSIF ( FIELD_CLASS = 'DOUBLE' ) THEN
                        FINAL_REQUEST := FINAL_REQUEST ||  VALUE_DOUBLE ;
                   ELSIF ( FIELD_CLASS = 'DATE' ) THEN
                        FINAL_REQUEST := FINAL_REQUEST ||  'TO_DATE(''' || VALUE_DATE ||''',''dd-mm-yyyy'')' ;
                   END IF;    --FIELD_CLASS
               END IF;
            END IF ; -- End else cas general VALUE_STRING NOT NULL
            FINAL_REQUEST := FINAL_REQUEST || ')';
          END IF; -- Critere champs

            LINE_COUNT := LINE_COUNT +1;

     END LOOP;
     --Spatial part
     IF (IS_SHAPE_ENABLE = 1 ) THEN
       --On isole la partie attributaire : traitement du cas ( Attri1 OR Attri2 ) AND GeoCrit
       IF LINE_COUNT > 0 THEN
		 FINAL_REQUEST := FINAL_REQUEST || ') intersect ';
       END IF;
       FINAL_REQUEST := FINAL_REQUEST || ' (SELECT distinct MG.ID_MASTERDATA FROM DMSGEOMETRY GEO,MASTERDATA_GEOM MG WHERE MG.ID_GEOMETRY = GEO.ID AND ';
       FINAL_REQUEST := FINAL_REQUEST || 'sdo_anyinteract (GEO.SHAPE,' ;
       FINAL_REQUEST := FINAL_REQUEST || '( SELECT R.SHAPE  FROM REQUEST R WHERE id = ' || REQUEST_ID ;
       -- We do a like instead of a equals because it's seem that there is an issue with this function on oracle 11g. Equals worked with the previous oracle version (10) and it may work with the news version of oravle (to be tested)
       FINAL_REQUEST := FINAL_REQUEST || ')) like ''%TRUE''' ;
       FINAL_REQUEST := FINAL_REQUEST || ')';
       IF LINE_COUNT = 0 THEN
	     FINAL_REQUEST := FINAL_REQUEST || ')';
	   END IF;
   	 ELSE
       FINAL_REQUEST := FINAL_REQUEST || ')';
     END IF;

      -- END REQUEST
      FINAL_REQUEST := FINAL_REQUEST || ')';

       REPLACE(VALUE_STRING, 'master_project_name', 'origin_project');

      DBMS_OUTPUT.PUT_LINE('REQUEST READY TO BE EXECUTED :  ' || FINAL_REQUEST);

      OPEN cur_OUT FOR FINAL_REQUEST;
      END;
  END query_master_data_proc;





  SELECT * FROM MASTERDATA MD WHERE MD.PROMOTE_STATUS = 'MASTER' AND MD.ID IN ((select ID_MASTERDATA from METADATA
  where ID_MASTERDATA in (select id from MASTERDATA where ID_MASTERPROJECT in (select id from MASTERPROJECT where NAME LIKE('%UK%'))