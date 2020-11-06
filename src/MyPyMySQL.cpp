#define BOOST_PYTHON_STATIC_LIB
#include <iostream>
#include <boost/python.hpp>
#include <mysql.h>
#include <string>
using namespace boost::python;
using namespace std;
class Option {
public:
    bool autocommit;
    bool log;

    Option(bool autocommit, bool log) {
        this->log = log;
        this->autocommit = autocommit;
    }

    Option() {
        this->autocommit = true;
        this->log = true;
    }
};

class Connection {
private:
    const char* host;
    const char* username;
    const char* password;
    int port;
    const char* database;
    MYSQL mysql;
    Option option;
    bool _mysql_init() {
        mysql_init(&mysql);
        mysql_real_connect(&this->mysql, this->host, this->username, this->password, this->database, this->port, nullptr, 0);
        if (mysql_error(&mysql) == nullptr) {
            return true;
        }
        return false;
    }
public:
    Connection(string host, string username, string password, string database, int port = 3306, bool autocommit = true,
               bool log = true) {
        this->username = username.c_str();
        this->password = password.c_str();
        this->port = port;
        this->database = database.c_str();
        this->host = host.c_str();
        this->option.log = log;
        this->option.autocommit = autocommit;
        _mysql_init();
    }
    dict execute(char* sql) {
        dict dic;
        MYSQL_RES* res;
        MYSQL_ROW rows;
        MYSQL_FIELD* field;
        int field_count = 0;
        int row_count = 0;
        mysql_query(&this->mysql, sql);
        res = mysql_store_result(&this->mysql);
        field_count = mysql_num_fields(res);
        field = mysql_fetch_fields(res);
        row_count = mysql_num_rows(res);
        boost::python::list field_name_list;
        for (int k = 0; k < field_count; k++) {
            field_name_list.append(str(string(field[k].name)));
        }
        boost::python::list tup;
        while (rows = mysql_fetch_row(res)) {
            boost::python::list item;
            for (int j = 0; j < field_count; j++) {
                item.append(str(string(rows[j])));
            }
            tup.append(item);
        }
        dic["field"] = field_name_list;
        dic["res"] = tup;
        if (this->option.log) {
            cout << "INFO\t" << sql << "\t successfully" << endl;
        }
        return dic;
    }

    void Log(bool t_or_f) {
        this->option.log = t_or_f;
    }

    void setOption(Option newOp) {
        this->option = newOp;
    }
};

BOOST_PYTHON_MODULE(MyPyMySQL) {
    class_<Connection>("Connection", init<string, string, string, string, int, bool, bool>())
            .def("execute", &Connection::execute, "execute(sql) -> dict")
            .def("setOption", &Connection::setOption, "setOption(option) -> None");
    class_<Option>("Option", init<bool, bool>());
}
