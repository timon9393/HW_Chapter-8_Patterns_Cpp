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
	explicit SqlSelectQueryBuilder() {}

	SqlSelectQueryBuilder& AddColumns(const std::vector<std::string>& columns) noexcept
	{
		for (const auto& el : columns)
			sql.column.push_back(el);
		return *this;
	}

	SqlSelectQueryBuilder& AddFrom(const std::string& table)
	{
		sql.table = table;
		return *this;
	}

	SqlSelectQueryBuilder& AddWhere(const std::map<std::string, std::string>& kv) noexcept
	{
		sql.wh = kv;
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
	std::vector<std::string> add_cols{ "name", "phone" };
	std::map<std::string, std::string> add_where = { {"id", "42"}, {"name", "John"} };
	query_builder.AddColumns(add_cols);
	query_builder.AddFrom("students");
	query_builder.AddWhere(add_where);

	std::cout << query_builder.BuildQuery();


	/*static_assert(query_builder.BuildQuery(),
					"SELECT name, phone FROM students WHERE id=42 AND name=John;");*/
};