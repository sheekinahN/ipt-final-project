#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    int i;
    for(i=0; i<argc; i++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

int main(int argc, char *argv[]){
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char *sql;
    char *name;
    char *email;
    char *message;

    /* Open database */
    rc = sqlite3_open("contacts.db", &db);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return(1);
    }

    /* Create SQL statement */
    sql = "CREATE TABLE IF NOT EXISTS contacts("  \
         "name TEXT NOT NULL," \
         "email TEXT NOT NULL," \
         "message TEXT NOT NULL);";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    /* Get the form data */
    name = getenv("QUERY_STRING");
    email = strstr(name, "email=");
    message = strstr(email, "message=");
    *email = '\0';
    *message = '\0';
    email += 6;
    message += 8;

    /* Insert the data into the database */
    sql = sqlite3_mprintf("INSERT INTO contacts (name, email, message) VALUES ('%q', '%q', '%q');", name, email, message);
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    if( rc != SQLITE_OK ){
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    } else {
        printf("Content-type:text/html\n\n");
        printf("<html>\n");
        printf("<head>\n");
        printf("<title>Thank You</title>\n");
        printf("</head>\n");
        printf("<body>\n");
        printf("<h1>Thank you for contacting us!</h1>\n");
        printf("</body>\n");
        printf("</html>\n");
    }

    sqlite3_close(db);
    return 0;
}
