#include <iostream>
#include "Wt/Dbo/Dbo.h"
#include "Wt/Dbo/backend/Postgres.h"
#include <Windows.h>
#include <vector>
#pragma execution_character_set("utf-8")
using namespace std;
class publisher;
class book;
class shop; 
class sale;

class stock
{
public:
	 //полей класса должно быть столько же , сколько в persist полей field - иначе сдвиг
	Wt::Dbo::ptr<book>id_book;
	Wt::Dbo::ptr<shop>id_shop;
	int  count;
	Wt::Dbo::collection<Wt::Dbo::ptr<sale>> sales;
	template<typename Action>
	void persist(Action& a) //кроме persist другие названия не подходят	
	{		//поле id создаётся автоматически
		Wt::Dbo::belongsTo(a, id_book, "book0");
		Wt::Dbo::belongsTo(a, id_shop, "shop0");
		Wt::Dbo::field(a, count, "count");
		Wt::Dbo::hasMany(a, sales, Wt::Dbo::ManyToOne, "stock0");
	}
};
class book {
public:
	std::string title;
	Wt::Dbo::ptr<publisher>id_publisher;
	Wt::Dbo::collection<Wt::Dbo::ptr<stock>> stocks;
	template<typename Action>
	void persist(Action& a)
	{
		Wt::Dbo::field(a, title, "title");
		Wt::Dbo::belongsTo(a, id_publisher, "publisher0");
		Wt::Dbo::hasMany(a, stocks, Wt::Dbo::ManyToOne, "book0");
	}
};
class publisher {
public:
	std::string name;
	Wt::Dbo::collection<Wt::Dbo::ptr<book>> books;
	template<typename Action>
	void persist(Action& a) {
		Wt::Dbo::field(a, name, "name");
		Wt::Dbo::hasMany(a, books, Wt::Dbo::ManyToOne, "publisher0");
	}
};

class shop {
public:
	std::string name;
	Wt::Dbo::collection<Wt::Dbo::ptr<stock>> stocks;
	template<typename Action>
	void persist(Action& a) {
		Wt::Dbo::field(a, name, "name");
		Wt::Dbo::hasMany(a, stocks, Wt::Dbo::ManyToOne, "shop0");
	}
};

class sale
{
public: 
	int prise;
	std::string date_sale;
	Wt::Dbo::ptr<stock> id_stock;
	int  count;	
	template<typename Action>
	void persist(Action& a)
	{
		Wt::Dbo::field(a, prise, "prise");
		Wt::Dbo::field(a, date_sale, "date_sale");
		Wt::Dbo::belongsTo(a, id_stock, "stock0");
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
		session.mapClass<book>("book0");
		session.mapClass<publisher>("publisher0");
		session.mapClass<shop>("shop0");
		session.mapClass<sale>("sale0");
		try {session.createTables();}		
		catch (exception& e) { cout << "table olready exists" << endl; }

		/*
		std::vector <unique_ptr<publisher>> x_p; //xp.resize(6); //работает
		std::string names[] = { "buka", "Thema", "Nauka", "Art", "Pressa", "Litera", "dayTime" };
		for(std::string x: names)
			x_p.emplace_back(move(new publisher{ x, {} })); //{} -это для collection - books

		Wt::Dbo::Transaction page_p { session };  //{}
		for(int t=0; t<sizeof(names)/sizeof(std::string); t++)
		session.add(move(x_p[t]));
		page_p.commit();	
		*/

		std::vector <unique_ptr<book>> x_b; //xp.resize(6); //работает
		std::string books1[] = {"Book1",  "Book", "Story", "Diktionary", "Book for keeds", "Meer" }; //книги для buka
		Wt::Dbo::Transaction page_b{ session };  //{}
		Wt::Dbo::ptr<publisher> pub = session.find<publisher>().where("name=buka");
		for (std::string x : books1)
			x_b.emplace_back(move(new book{ x, pub, {} })); //{} -это для collection - stocks		
		
		for (int t = 0; t < sizeof(books1) / sizeof(std::string); t++)
			session.add(move(x_b[t]));
		page_b.commit();

	/*	unique_ptr<stock> x1{new stock {22, 11, 15}}; // без delete  так как unique_ptr
		unique_ptr<stock> x0 = make_unique<stock>();		
		x0->id_book = 2;
		x0->id_shop = 3;
		x0->count = 4;

		Wt::Dbo::Transaction page0(session);
		session.add(move(x0));
		Wt::Dbo::ptr<stock>x5 = session.add(move(x1));		
		page0.commit();

		Wt::Dbo::ptr<stock> new_book = session.find<stock>().where("id_book = ?").bind(22);
		unique_ptr<book> b1{ new book {"Stories", new_book}};
		Wt::Dbo::Transaction page_b(session);
		Wt::Dbo::ptr<book>b5 = session.add(move(b1));
		page_b.commit();*/

	}
	catch (exception& e) {
		cout << e.what() << endl;
	}
}