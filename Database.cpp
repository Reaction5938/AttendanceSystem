#include "Database.h"

Database::Database(){
    conn = nullptr;
}

bool Database::Connect(){

    conn = PQconnectdb(
        
        "host= YOUR_HOST "
        
        "port=5432"

        "dbname= YOUR_DATEBASE "

        "user=YOUR_USER "

        "password=YOUR_PASSWORD "
    );

    if(PQstatus(conn) != CONNECTION_OK){
        
        wxMessageBox(wxString::FromUTF8(PQerrorMessage(conn)),
            wxString::FromUTF8("接続エラー"));

        PQfinish(conn);

        conn = nullptr;

        return false;
    }

    return true;
}

bool Database::AddUser(const wxString& userName, const wxString& password){
    
    wxString sql = wxString::Format(
        "INSERT INTO users(user_name, password) "
        "VALUES('%s', '%s');",
        userName.c_str(),
        password.c_str()
    );

    PGresult* result = PQexec(conn, sql.utf8_str());

    if(PQresultStatus(result) != PGRES_COMMAND_OK){
        
        const char* sqlState = PQresultErrorField(result, PG_DIAG_SQLSTATE);

        if(sqlState && strcmp(sqlState, "23505") == 0){

            wxMessageBox(
                wxString::FromUTF8("同じユーザー名とパスワードは登録できません。"),
                wxString::FromUTF8("登録")
            );
        }
        else{

            wxMessageBox(
                wxString::FromUTF8(PQerrorMessage(conn)),
                wxString::FromUTF8("SQLエラー")
            );
        }

        PQclear(result);

        return false;
    }

    PQclear(result);

    return true;
}


bool Database::Login(const wxString& userName, const wxString& password){

    wxString sql = wxString::Format(
        "SELECT * FROM users "
        "WHERE user_name = '%s' "
        "AND password = '%s';",
        userName.c_str(),
        password.c_str()
    );

    PGresult* result = PQexec(conn, sql.utf8_str());

    if(PQresultStatus(result) != PGRES_TUPLES_OK){

        wxMessageBox(
            wxString::FromUTF8(PQerrorMessage(conn)),
            wxString::FromUTF8("SQLエラー")
        );

        PQclear(result);

        return false;
    }

    bool success = (PQntuples(result) == 1);

    PQclear(result);

    return success;
}

bool Database::DeleteUser(const wxString& userName, const wxString& password){

    wxString sql = wxString::Format(
        "DELETE FROM users "
        "WHERE user_name = '%s' "
        "AND password = '%s';",
        userName.c_str(),
        password.c_str()
    );

    PGresult* result = PQexec(conn, sql.utf8_str());

    if(PQresultStatus(result) != PGRES_COMMAND_OK){
        wxMessageBox(wxString::FromUTF8(PQerrorMessage(conn)),
        wxString::FromUTF8("SQLエラー"));

        PQclear(result);
        
        return false;
    }

    if(wxString(PQcmdTuples(result)) == "0"){

        PQclear(result);

        wxMessageBox(
            wxString::FromUTF8("ユーザー名またはパスワードが違います。"),
            wxString::FromUTF8("削除")
        );

        return false;
    }

    PQclear(result);

    return true;

}

wxArrayString Database::GetUsers(){

    wxArrayString users;

    PGresult* result = PQexec(conn, "SELECT user_name FROM users;");

    if(PQresultStatus(result) != PGRES_TUPLES_OK){

        PQclear(result);

        return users;
    }

    int rows = PQntuples(result);

    for(int i = 0; i < rows; i++){

        users.Add(wxString::FromUTF8(PQgetvalue(result, i, 0)));
    }

    PQclear(result);
    
    return users;
}

int Database::GetUserId(const wxString& userName, const wxString& password){

    wxString sql = wxString::Format(
        "SELECT user_id "
        "FROM users "
        "WHERE user_name = '%s' "
        "AND password = '%s';",
        userName.c_str(),
        password.c_str()
    );

    PGresult* result = PQexec(conn, sql.utf8_str());

    if(PQresultStatus(result) != PGRES_TUPLES_OK){

        PQclear(result);

        return -1;
    }


    if(PQntuples(result) == 0){

        PQclear(result);

        return -1;
    }

    int userId = atoi(PQgetvalue(result, 0, 0));

    PQclear(result);

    return userId;
}

bool Database::ClockIn(const AttendanceRecord& record){

    wxString sql = wxString::Format(
        "INSERT INTO attendance(user_id, clock_in) "
        "VALUES(%d, '%s');",

        record.userId,

        record.clockInTime.c_str()
    );

    PGresult* result = PQexec(conn, sql.utf8_str());

    if(PQresultStatus(result) != PGRES_COMMAND_OK){

        wxMessageBox(
            wxString::FromUTF8(PQerrorMessage(conn)),
            wxString::FromUTF8("SQLエラー")
        );

        PQclear(result);

        return false;
    }

    PQclear(result);

    return true;
}

bool Database::ClockOut(const AttendanceRecord& record){

    const char* checkSql = 
        "SELECT vacation_type "
        "FROM attendance "
        "WHERE user_id = $1::integer "
        "AND clock_out IS NULL "
        "ORDER BY attendance_id DESC "
        "LIMIT 1;";

    wxString userIdText = wxString::Format("%d", record.userId);

    const char* checkValues[] = { userIdText.utf8_str() };

    PGresult* checkResult = PQexecParams(
        conn,
        checkSql,
        1,
        nullptr,
        checkValues,
        nullptr,
        nullptr,
        0
    );

    if(PQresultStatus(checkResult) == PGRES_TUPLES_OK && PQntuples(checkResult) > 0){
        if(!PQgetisnull(checkResult, 0, 0)){
            const char* vacationType = PQgetvalue(checkResult, 0, 0);

            if(strlen(vacationType) > 0){
                wxMessageBox(
                    wxString::FromUTF8("勤怠申請が提出されているため、退勤登録はできません。"),
                    wxString::FromUTF8("エラー"),
                    wxOK | wxICON_WARNING
                );

                PQclear(checkResult);
                return false;
            }
        }
    }

    PQclear(checkResult);

    wxString sql = wxString::Format(
        "UPDATE attendance "
        "SET clock_out = '%s' "
        "WHERE attendance_id = ("
            "SELECT attendance_id "
            "FROM attendance "
            "WHERE user_id = %d "
            "AND clock_out IS NULL "
            "ORDER BY attendance_id DESC "
            "LIMIT 1"
        ");",

        record.clockOutTime.c_str(),
        record.userId
    );

    PGresult* result = PQexec(conn, sql.utf8_str());

    if(PQresultStatus(result) != PGRES_COMMAND_OK){

        wxMessageBox(
            wxString::FromUTF8(PQerrorMessage(conn)),
            wxString::FromUTF8("SQLエラー")
        );

        PQclear(result);

        return false;
    }

    PQclear(result);

    return true;
}

bool Database::SubmitVacationLeave(
    const wxString& userName,
    const wxString& date,
    const wxString& message
){

    PGresult* userResult = PQexec(
        conn,
        wxString::Format(
            "SELECT user_id FROM users WHERE user_name = '%s';",
            userName.c_str()
        ).utf8_str()
    );

    if(PQresultStatus(userResult) != PGRES_TUPLES_OK || PQntuples(userResult) == 0){
        PQclear(userResult);
        return false;
    }

    int userId = atoi(PQgetvalue(userResult, 0, 0));
    PQclear(userResult);

    wxString sql = wxString::Format(
        "INSERT INTO attendance(user_id, clock_in, vacation_type) "
        "VALUES(%d, '%s 00:00:00', '%s');",
        userId,
        date.c_str(),
        message.c_str()
    );

    PGresult* result = PQexec(conn, sql.utf8_str());

    if(PQresultStatus(result) != PGRES_COMMAND_OK){
        wxMessageBox(
            wxString::FromUTF8(PQerrorMessage(conn)),
            wxString::FromUTF8("SQLエラー")
        );
        PQclear(result);
        return false;
    }

    PQclear(result);
    return true;
}

bool Database::UpdateAttendance(
    int userId,
    const wxString& date,
    const wxString& time,
    bool updateClockIn,
    const wxString& correctionMessage
){
    
    const char* sql = nullptr;

    if(updateClockIn){
        sql = "UPDATE attendance "
              "SET clock_in = $1::timestamp, "
              "correction_message = $2 "
              "WHERE attendance_id = ("
              "SELECT attendance_id "
              "FROM attendance "
              "WHERE user_id = $3::integer "
              "AND clock_in::date = $4::date "
              "ORDER BY attendance_id DESC "
              "LIMIT 1"
              ");";
    }
    else{
        sql = "UPDATE attendance "
              "SET clock_out = $1::timestamp, "
              "correction_message = $2 "
              "WHERE attendance_id = ("
              "SELECT attendance_id "
              "FROM attendance "
              "WHERE user_id = $3::integer "
              "AND clock_in::date = $4::date "
              "ORDER BY attendance_id DESC "
              "LIMIT 1"
              ");";
    }

    wxString dateTime = date + " " + time;

    wxString userIdText = wxString::Format("%d", userId);

    const char* values[] = {
        dateTime.utf8_str(),
        correctionMessage.utf8_str(),
        userIdText.utf8_str(),
        date.utf8_str()
    };

    PGresult* result = PQexecParams(
        conn,
        sql,
        4,
        nullptr,
        values,
        nullptr,
        nullptr,
        0
    );

    if(PQresultStatus(result) != PGRES_COMMAND_OK){
        wxMessageBox(
            wxString::FromUTF8(PQerrorMessage(conn)),
            wxString::FromUTF8("SQLエラー")
        );

        PQclear(result);
        return false;
    }

    bool updated = wxString(PQcmdTuples(result)) != "0";

    PQclear(result);

    if(updated){
        return true;
    }

    if(!updateClockIn){
        wxMessageBox(
            wxString::FromUTF8("指定した日付に出勤データがありません。" "先に出勤を登録してください。"),
            wxString::FromUTF8("勤怠修正"),
            wxOK | wxICON_WARNING
        );

        return false;
    }

    const char* insertSql = "INSERT INTO attendance "
                            "(user_id, clock_in, correction_message) "
                            "VALUES ($1::integer, $2::timestamp, $3);";

    const char* insertValues[] = {
        userIdText.utf8_str(),
        dateTime.utf8_str(),
        correctionMessage.utf8_str()
    };

    PGresult* insertResult = PQexecParams(
        conn,
        insertSql,
        3,
        nullptr,
        insertValues,
        nullptr,
        nullptr,
        0
    );

    if(PQresultStatus(insertResult) != PGRES_COMMAND_OK){
        wxMessageBox(
            wxString::FromUTF8(PQerrorMessage(conn)),
            wxString::FromUTF8("SQLエラー")
        );

        PQclear(insertResult);

        return false;
    }

    PQclear(insertResult);

    return true;
}

wxVector<AttendanceRecord> Database::GetAttendanceRecords(){

   wxVector<AttendanceRecord> records;

   PGresult* result = PQexec(
        conn,
        "SELECT a.attendance_id, a.user_id, u.user_name, a.clock_in, a.clock_out, a.correction_message, a.vacation_type, a.overtime_message "
        "FROM attendance a "
        "JOIN users u ON a.user_id = u.user_id "
        "ORDER BY a.clock_in ASC;"
   );

   if(PQresultStatus(result) != PGRES_TUPLES_OK){
    
        wxMessageBox(
            wxString::FromUTF8(PQerrorMessage(conn)),
            wxString::FromUTF8("SQLエラー")
        );

        PQclear(result);

        return records;
   }

   int rows = PQntuples(result);

   for(int i = 0; i < rows; i++){

        AttendanceRecord record;

        record.attendanceId = atoi(PQgetvalue(result, i, 0));

        record.userId = atoi(PQgetvalue(result, i, 1));

        record.userName = wxString::FromUTF8(PQgetvalue(result, i, 2));

        record.clockInTime = wxString::FromUTF8(PQgetvalue(result, i, 3));

        if(PQgetisnull(result, i, 4)){

            record.clockOutTime = "";
        }
        else{

            record.clockOutTime = wxString::FromUTF8(PQgetvalue(result, i, 4));
        }

        if(PQgetisnull(result, i, 5)){

            record.correctMessage = "";
        }
        else{
            record.correctMessage = wxString::FromUTF8(PQgetvalue(result, i, 5));
        }

        if(PQgetisnull(result, i, 6)){
            record.vacationMessage = "";
        }
        else{
            record.vacationMessage = wxString::FromUTF8(PQgetvalue(result, i, 6));
        }

        if(PQgetisnull(result, i, 7)){
            record.overtimeMessage = "";
        }
        else{
            record.overtimeMessage = wxString::FromUTF8(PQgetvalue(result, i, 7));
        }


        records.push_back(record);
   }

   PQclear(result);

   return records;
}

wxArrayString Database::GetMissingClockOutDates(int userId){

    wxArrayString dates;

    const char* sql =
        "SELECT clock_in::date "
        "FROM attendance "
        "WHERE user_id = $1::integer "
        "AND clock_out IS NULL "
        "AND (vacation_type IS NULL OR vacation_type = '')"
        "AND clock_in::date < CURRENT_DATE "
        "ORDER BY clock_in ASC;";

    wxString userIdText = wxString::Format("%d", userId);

    const char* values[] = {
        userIdText.utf8_str()

    };

    PGresult* result = PQexecParams(
        conn,
        sql,
        1,
        nullptr,
        values,
        nullptr,
        nullptr,
        0
    );

    if(PQresultStatus(result) != PGRES_TUPLES_OK){

        wxMessageBox(
            wxString::FromUTF8(PQerrorMessage(conn)),
            wxString::FromUTF8("SQLエラー")
        );

        PQclear(result);

        return dates;
    }

    int rows = PQntuples(result);

    for(int i = 0; i < rows; i++){

        dates.Add(
            wxString::FromUTF8(PQgetvalue(result, i, 0))
        );
    }

    PQclear(result);

    return dates;
}

wxArrayString Database::GetOvertimeDates(int userId){

    wxArrayString dates;

    const char* sql =
        "SELECT clock_in::date "
        "FROM attendance "
        "WHERE user_id = $1::integer "
        "AND clock_out IS NOT NULL "
        "AND (vacation_type IS NULL OR vacation_type = '') "
        "AND (clock_out - clock_in - INTERVAL '1 hours') > INTERVAL '8 hours' "
        "ORDER BY clock_in ASC;";

    wxString userIdText = wxString::Format("%d", userId);

    const char* values[] = {
        userIdText.utf8_str()
    };

    PGresult* result = PQexecParams(
        conn,
        sql,
        1,
        nullptr,
        values,
        nullptr,
        nullptr,
        0
    );

    if(PQresultStatus(result) != PGRES_TUPLES_OK){

        wxMessageBox(
            wxString::FromUTF8(PQerrorMessage(conn)),
            wxString::FromUTF8("SQLエラー")
        );

        PQclear(result);

        return dates;
    }

    int rows = PQntuples(result);

    for(int i = 0; i < rows; i++){

        dates.Add(
            wxString::FromUTF8(PQgetvalue(result, i, 0))
        );
    }

    PQclear(result);

    return dates;
}

bool Database::SubmitOvertime(const wxString& userName, const wxString& date, const wxString& message){

    // 1. ユーザーIDの取得
    PGresult* userResult = PQexec(
        conn,
        wxString::Format("SELECT user_id FROM users WHERE user_name = '%s';", userName.c_str()).utf8_str()
    );

    if(PQresultStatus(userResult) != PGRES_TUPLES_OK || PQntuples(userResult) == 0){
        PQclear(userResult);
        return false;
    }

    int userId = atoi(PQgetvalue(userResult, 0, 0));
    PQclear(userResult);

    wxString userIdText = wxString::Format("%d", userId);

    // 2. 指定日の勤怠データが存在するか確認
    const char* checkSql = "SELECT attendance_id FROM attendance WHERE user_id = $1::integer AND clock_in::date = $2::date;";
    const char* checkValues[] = { userIdText.utf8_str(), date.utf8_str() };

    PGresult* checkResult = PQexecParams(
        conn,
        checkSql,
        2,
        nullptr,
        checkValues,
        nullptr,
        nullptr,
        0
    );

    if(PQresultStatus(checkResult) == PGRES_TUPLES_OK && PQntuples(checkResult) > 0){
        // --- 既にレコードが存在する場合 (UPDATE) ---
        PQclear(checkResult);

        const char* updateSql = "UPDATE attendance SET overtime_message = $1 "
                                "WHERE user_id = $2::integer AND clock_in::date = $3::date;";
        
        // $1 = message, $2 = userId, $3 = date
        const char* values[] = { message.utf8_str(), userIdText.utf8_str(), date.utf8_str() };

        PGresult* res = PQexecParams(
            conn,
            updateSql,
            3,
            nullptr,
            values,
            nullptr,
            nullptr,
            0
        );

        bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);
        PQclear(res);
        return success;

    } else {
        // --- レコードが存在しない場合 (INSERT) ---
        PQclear(checkResult);

        const char* insertSql = "INSERT INTO attendance (user_id, clock_in, overtime_message) "
                                "VALUES ($1::integer, $2::timestamp, $3);";
        
        wxString clockInTimeStamp = date + " 00:00:00";

        // $1 = userId, $2 = clockInTimeStamp, $3 = message
        const char* values[] = { userIdText.utf8_str(), clockInTimeStamp.utf8_str(), message.utf8_str() };

        PGresult* res = PQexecParams(
            conn,
            insertSql,
            3,
            nullptr,
            values,
            nullptr,
            nullptr,
            0
        );

        bool success = (PQresultStatus(res) == PGRES_COMMAND_OK);
        PQclear(res);
        return success;
    }

}