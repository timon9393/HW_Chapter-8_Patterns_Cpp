#include <iostream>
#include <map>
#include <vector>

struct SqlSelectQuery
{
	std::vector<std::string> column;
	std::string table;
	std::string name;
	int age;
	size_t id;
	std::string phone;
	std::map<std::string, std::string> wh;
};

class SqlSelectQueryBuilder
{
public:
	explicit SqlSelectQueryBuilder(){}

	SqlSelectQueryBuilder& AddColumn(const std::string& col)
	{
		sql.column.push_back(col);
		return *this;
	}

	SqlSelectQueryBuilder& AddFrom(const std::string& table)
	{
		sql.table = table;
		return *this;
	}

	SqlSelectQueryBuilder& AddWhere(const std::string& key, const std::string& value)
	{
		sql.wh[key] = value;
		return *this;
	}

	std::string BuildQuery()
	{
		if (!sql.column.empty())
		{
			for (const auto& el : sql.column)
			{
				if (&el == &sql.column.back())
					select_ += el;
				else
					select_ += el + ", ";
			}
		}
		else
			select_ = "*";

		int i = 0;
		for (const auto& el : sql.wh)
		{
			if (i == sql.wh.size() - 1)
				where_ += el.first + "=" + el.second;
			else
				where_ += el.first + "=" + el.second + " AND ";
			
			i++;
		}

		return "SELECT " + select_ + " FROM " + sql.table + " WHERE " + where_ + ";";
	}

private:
	SqlSelectQuery sql;
	std::string select_;
	std::string where_;
};

int main() 
{
	SqlSelectQueryBuilder query_builder;
	query_builder.AddColumn("name").AddColumn("phone");
	query_builder.AddFrom("students");
	query_builder.AddWhere("id", "42").AddWhere("name", "John");

	std::cout << query_builder.BuildQuery();


	/*static_assert(query_builder.BuildQuery(),
					"SELECT name, phone FROM students WHERE id=42 AND name=John;");*/
};