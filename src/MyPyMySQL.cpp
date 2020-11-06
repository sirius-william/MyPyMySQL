#define BOOST_PYTHON_STATIC_LIB
#include <iostream>
#include <boost/python.hpp>
#include <mysql.h>
#include <string>
using namespace boost::python;
using namespace std;

enum LogType {
	STATUS,
	INFO,
	FATAL,
	WARNING,
};

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


	bool _mysql_init() {
		mysql_init(&mysql);
		mysql_real_connect(&this->mysql, this->host, this->username, this->password, this->database, this->port,
		                   nullptr, 0);
		if (mysql_errno(&mysql) == 0) {
			mysql_autocommit(&this->mysql, this->option.autocommit); // 设置是否自动提交事务
			if (mysql_errno(&this->mysql) == 0) {
				return true;
			}
		}
		return false;
	}

	void printLog(string logMsg, LogType type) {
		if (this->option.log) {
			switch (type) {
			case STATUS: {
				cout << "STATUS\t";
				break;
			}
			case INFO: {
				cout << "INFO\t";
				break;
			}
			case FATAL: {
				cout << "FATAL\t";
				break;
			}
			case WARNING: {
				cout << "WARNING\t";
				break;
			}
			}
			cout << logMsg << endl;
		}

	}


public:
	Option option;

	Connection(string host, string username, string password, string database, int port = 3306, bool autocommit = true,
	           bool log = true) {
		this->username = username.c_str();
		this->password = password.c_str();
		this->port = port;
		this->database = database.c_str();
		this->host = host.c_str();
		this->option = Option{autocommit, log};
		_mysql_init();
	}

	Connection(string host, string username, string password, string database, int port = 3306,
	           Option opt = Option{true, true}) {
		this->username = username.c_str();
		this->password = password.c_str();
		this->port = port;
		this->database = database.c_str();
		this->host = host.c_str();
		this->option = opt;
		_mysql_init();
	}

	dict execute(char* sql) {
		dict dic; // 结果集字典
		boost::python::list field_name_list; // 字段列表
		boost::python::list res_list;
		MYSQL_RES* res;
		MYSQL_ROW rows;
		MYSQL_FIELD* field;
		int field_count = 0;
		int row_count = 0;
		int affect = 0;
		try {
			mysql_query(&this->mysql, sql);

			if (mysql_errno(&this->mysql) != 0) {
				cout << "if mysql_error" << endl;
				printLog("execute\t" + string(sql) + "\tunsuccessfully cause:\t" + string(mysql_error(&this->mysql)),
				         FATAL);
				dic["error"] = str("error");
				commit();
				return dic;
			}
			res = mysql_store_result(&this->mysql);
			if (!res) {
				affect = mysql_affected_rows(&this->mysql);
				printLog(
					"execute SQL not select statement:\t" + string(sql) + "\taffect rows:" + std::to_string(affect),
					INFO);
				dic["affect_rows"] = affect;
				return dic;
			}
			field_count = mysql_num_fields(res);
			field = mysql_fetch_fields(res);
			row_count = mysql_num_rows(res);

			for (int k = 0; k < field_count; k++) {
				field_name_list.append(str(string(field[k].name)));
			}
			while (rows = mysql_fetch_row(res)) {
				boost::python::list item;
				for (int j = 0; j < field_count; j++) {
					item.append(str(string(rows[j])));
				}
				res_list.append(item);
			}
			dic["field"] = field_name_list;
			dic["res"] = res_list;
			printLog("execute\t" + string(sql) + "\tsuccessfully", INFO);
			return dic;
		}
		catch (exception e) {
			cout << e.what() << endl;
			dic["err"] = str(e.what());
			return dic;
		}
	}

	void Log(bool t_or_f) {
		this->option.log = t_or_f;
	}

	void setOption(Option newOp) {
		this->option = newOp;
	}

	void commit() {
		if (!this->option.autocommit) {
			mysql_commit(&this->mysql);
		}
	}
};

BOOST_PYTHON_MODULE(MyPyMySQL) {
	class_<Connection>("Connection",
	                   "Connection(host, username, password, database, port, autocommit, isLog)\n\nConnection(host, username, password, database, port, Option)",
	                   init<string, string, string, string, int, bool, bool>())
		.def(init<string, string, string, string, int, Option>())
		.def("execute", &Connection::execute, "execute(sql) -> dict")
		.def("setOption", &Connection::setOption, "setOption(option) -> None")
		.def("commit", &Connection::commit, "commit() -> None")
		.add_property("option", &Connection::option, "Option");
	class_<Option>("Option", "Option(autocommit, isLog)", init<bool, bool>())
		.add_property("autocommit", &Option::autocommit, "autocommit")
		.add_property("islog", &Option::log, "isLog");
}
