CREATE FUNCTION get_domain_type_id(text) RETURNS integer AS $$
	DECLARE
		t_type ALIAS FOR $1;
		i_id_domain_type integer;
	BEGIN
		SELECT INTO i_id_domain_type id FROM domain_types WHERE type = t_type;

		IF NOT FOUND THEN
		   	  RETURN -1;
		END IF;
		
		RETURN i_id_domain_type;
	END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION get_domain_id(text) RETURNS integer AS $$
    DECLARE
        t_domain ALIAS FOR $1;
        i_id_domain  integer;
    BEGIN
        SELECT INTO i_id_domain id FROM domains WHERE domain=t_domain;
            
        IF NOT FOUND THEN   
              RETURN -1;
        END IF;

        RETURN i_id_domain;
    END;
$$ LANGUAGE plpgsql;

CREATE FUNCTION spam_user(text,text,integer) RETURNS integer AS $$
	   DECLARE
			t_login ALIAS FOR $1;
			t_domain ALIAS FOR $2;
	   		i_score integer;
			i_type ALIAS FOR $3;
	   BEGIN
			SELECT INTO i_score spam_score FROM users_view where login=t_login and domain=t_domain and id_spam=i_type;
	   		
			IF NOT FOUND THEN
               RETURN 1000;
        	END IF;

			RETURN i_score*10;
	   END;
$$ LANGUAGE plpgsql;
