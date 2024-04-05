#include <iostream>
#include "Wt/Dbo/Dbo.h"
#include "Wt/Dbo/backend/Postgres.h"
#include <Windows.h>
#pragma execution_character_set("utf-8")
using namespace std;

class stock
{
public:
	int id, id_book, id_shop, count;
	template<typename Action>
	void persist(Action& a) //кроме persist другие названия не подходят
	{
		Wt::Dbo::field(a, id, "id0");
		Wt::Dbo::field(a, id_book, "id_book");
		Wt::Dbo::field(a, id_shop, "id_shop");
		Wt::Dbo::field(a, count, "count");
	}
};

int main()
{
	//setlocale(LC_ALL, "Russian");
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	try {
		std::cout << "ORM\n";
		string c = "host=localhost " //обязательно пробел
			"port=5432 " //стандартный порт psql
			"dbname=Task5 "
			"user=pq "
			"password=zxc123";
		auto connection = make_unique<Wt::Dbo::backend::Postgres>(c);
		Wt::Dbo::Session session;
		session.setConnection(move(connection));
		session.mapClass<stock>("stock0");
		try {
			session.createTables();
		}
		catch (exception e) { cout << "table olready exists" << endl; }

		Wt::Dbo::Transaction page0(session);
		unique_ptr<stock> x1{ new stock {22, 11, 15, 65} };
		/*unique_ptr<stock> x0 = make_unique<stock>();
		x0->id = 1;
		x0->id_book = 2;
		x0->id_shop = 3;
		x0->count = 4;*/

		
		Wt::Dbo::ptr<stock>x5 = session.add(move(x1));
		page0.commit();


	}
	catch (exception e) {
		cout << e.what() << endl;
	}
}