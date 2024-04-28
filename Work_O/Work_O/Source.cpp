#include <iostream>
#include "Wt/Dbo/Dbo.h"
#include "Wt/Dbo/backend/Postgres.h"
#include <Windows.h>
#include <string>
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
	try { session.createTables(); }
	catch (exception& e) { cout << "table olready exists" << endl; }


	std::vector <unique_ptr<publisher>> x_p;  //работает
	std::string names[] = { "buka", "Thema", "Nauka", "Art", "Pressa", "Litera", "dayTime" };
	/*for (std::string x : names)
		x_p.emplace_back(move(new publisher{ x, {} })); //{} -это для collection - books

	Wt::Dbo::Transaction page_p { session };  //{}
	for(int t=0; t<sizeof(names)/sizeof(std::string); t++)
	session.add(move(x_p[t]));
	page_p.commit();*/


	std::vector <unique_ptr<shop>> x_sh;  //работает
	std::string names_sh[] = { "Azbuka", "World reading" };
	/*for (std::string x : names_sh)
		x_sh.emplace_back(move(new shop{ x, {} })); //{} -это для collection - stoks

	Wt::Dbo::Transaction page_sh{ session };  //{}
	for (int t = 0; t < sizeof(names_sh) / sizeof(std::string); t++)
		session.add(move(x_sh[t]));
	page_sh.commit();*/



	std::vector<std::vector<unique_ptr<book>>> x_b_(7); //работает
	//издтельства-книги
	std::vector<string> books1 = { "Book1",  "Book", "Story", "Diktionary", "Book for keeds", "Meer" }; //книги для buka
	std::vector<string> books2 = { "Book #1", "like summer" }; //Thema
	std::vector<string> books3 = { "Book for me", "Many stories about future" }; //Nauka
	std::vector<string> books4 = { "Book for girls", "Many stories" }; //Art
	std::vector<string> books5 = { "Journal", "Journal of analytics" };//Pressa
	std::vector<string> books6 = { "Muzik" };//Litera
	std::vector<string> books7 = { "New folders" };//dayTime
	std::vector<std::vector<string>> book_all = { books1, books2, books3, books4, books5, books6, books7 };
	//std::string names[] = { "buka", "Thema", "Nauka", "Art", "Pressa", "Litera", "dayTime" };
/*std::vector<Wt::Dbo::ptr<publisher>> pub(7);
{
	Wt::Dbo::Transaction page_b{ session };
	for (int i = 0; i < book_all.size(); i++) {
	std::vector<string> g = book_all[i];
	pub[i] = session.find<publisher>().where("name=?").bind(names[i]);
	for (int t = 0; t < g.size(); t++) {
		x_b_[i].emplace_back(move(new book{g[t], pub[i], {}})); //{} -это для collection - stocks
		session.add(move(x_b_[i][t]));
	}	}
	page_b.commit();
}*/
	
	{
		Wt::Dbo::Transaction page_sh{ session }; //работает
		unique_ptr<stock> v1{ new stock{{},{},3} };
		unique_ptr<stock> v2{ new stock{{},{},7} };
		/*Wt::Dbo::ptr<book> b1=session.find<book>().where("title = ?").bind("Story");
		Wt::Dbo::ptr<book> b2 = session.find<book>().where("title = ?").bind("Muzik");
		Wt::Dbo::ptr<shop> s1 = session.find<shop>().where("name = ?").bind("Azbuka");
		Wt::Dbo::ptr<shop> s2 = session.find<shop>().where("name = ?").bind("World reading");
		Wt::Dbo::ptr<stock> v1_ptr = session.add(move(v1));
		Wt::Dbo::ptr<stock> v2_ptr = session.add(move(v2));
		v1_ptr.modify()->id_book = b1;
		v1_ptr.modify()->id_shop = s1;
		v2_ptr.modify()->id_book = b2;
		v2_ptr.modify()->id_shop = s2;*/
		page_sh.commit();
	}

	Wt::Dbo::Transaction page_sale{ session };//работает
	unique_ptr <sale> sa1{ new sale{ 4,"2012-10-23",{},7 } };
	unique_ptr <sale> sa2{ new sale{ 2,"2011-15-13",{},7 } };
	unique_ptr <sale> sa3{ new sale{ 1,"2012-05-22",{},7 } };
	unique_ptr <sale> sa4{ new sale{ 1,"2012-11-03",{},7 } };
	/*Wt::Dbo::ptr<sale> sa1_ptr = session.add(move(sa1));
	Wt::Dbo::ptr<sale> sa2_ptr = session.add(move(sa2));
	Wt::Dbo::ptr<sale> sa3_ptr = session.add(move(sa3));
	Wt::Dbo::ptr<sale> sa4_ptr = session.add(move(sa4));
	Wt::Dbo::ptr<stock> st1_ptr = session.find<stock>().where("count = ?" ).bind(3);
	Wt::Dbo::ptr<stock> st2_ptr = session.find<stock>().where("count = ?").bind(7);
	sa1_ptr.modify()->id_stock = st1_ptr;
	sa2_ptr.modify()->id_stock = st1_ptr;
	sa3_ptr.modify()->id_stock = st2_ptr;
	sa4_ptr.modify()->id_stock = st2_ptr;*/
	page_sale.commit();

	
	//ищем магазины с заданным издателем buka или Litera  от других издателей нет завоза
	std::cout << "Insert one of names to publisher: buka, Thema, Nauka, "
		"Art, Pressa, Litera, dayTime:   ";
	std::string publisher_name;
	std::cin >> publisher_name;
	Wt::Dbo::Transaction sel{session};
	Wt::Dbo::ptr<publisher> p_ptr = session.find<publisher>().where("name = ?").bind(publisher_name);
	Wt::Dbo::collection<Wt::Dbo::ptr<book>> b_ptrs = session.find<book>();
	std::vector<Wt::Dbo::ptr<book>> b_pub;
	std::vector<Wt::Dbo::ptr<stock>>st_pub;
	std::vector<Wt::Dbo::ptr<shop>>sh_pub;
	for (auto& bu : b_ptrs)
	{
		if (p_ptr == bu->id_publisher)
		{
			b_pub.emplace_back(bu);
			//std::cout << bu->title << " ";
		}//std::cout << std::endl;
	}
	Wt::Dbo::collection<Wt::Dbo::ptr<stock>> st_ptrs = session.find<stock>();
	for (auto& su : st_ptrs)
	{
		for (auto& bu : b_pub)
			if (su->id_book == bu)
			{
				if (su->id_shop)
					sh_pub.emplace_back(su->id_shop);
				st_pub.emplace_back(su);
				//std::cout << su->count << " ";
			}//std::cout << std::endl;
	}
	Wt::Dbo::collection<Wt::Dbo::ptr<shop>> sh_ptrs = session.find<shop>();
	int t = 0;
	for (auto& shu : sh_ptrs) {
		for (auto& x : sh_pub) {		
			if (shu->name == x->name)
			{
				t++;
				std::cout << x->name << " ";
				break;
			}
		}
	}
	if (t == 0)
		std::cout << "Not found any shop.";
	sel.commit();

	}catch (exception& e) {
				cout << e.what() << endl;
			}
	}
