
#include <cassert>
#include <tuple>
#include <algorithm>

#include "Container.h"


Person::Person(const char * name, const char * surname, size_t id)
	: name_(name),
	surname_(surname),
	id_(id)
{}

Person::Person(const std::string & name, const std::string & surname, size_t id)
	: Person(name.data(), surname.data(), id)
{}

const std::string & Person::Name() const
{
	return name_;
}

const std::string & Person::Surname() const
{
	return surname_;
}

size_t Person::ID() const
{
	return id_;
}

void Person::SetName(const char * name)
{
	name_ = name;
}

void Person::SetName(const std::string & name)
{
	SetName(name.data());
}

void Person::SetSurname(const char * surname)
{
	surname_ = surname;
}

void Person::SetSurname(const std::string & surname)
{
	SetSurname(surname.data());
}

void Person::SetID(size_t id)
{
	id_ = id;
}

bool Person::operator<(const Person & other) const
{
	size_t id1 = ID(),
		id2 = other.ID();

	return std::tie(Name(), Surname(), id1) <
		std::tie(other.Name(), other.Surname(), id2);
}

inline size_t Container::Size() const
{
	return people_.size();
}

Person & Container::Get(size_t indx)
{
	assert(indx < Size() && "Invalid index");
	return people_[indx];
}

const Person & Container::Get(size_t indx) const
{
	assert(indx < Size() && "Invalid index");
	return people_[indx];
}

Person & Container::operator[](size_t indx)
{
	return Get(indx);
}

const Person & Container::operator[](size_t indx) const
{
	return Get(indx);
}

void Container::Insert(Person && newPerson, size_t indx_before)
{
	decltype(people_)::const_iterator iter = indx_before != std::numeric_limits<size_t>::max() ?
		std::next(people_.cbegin(), indx_before) : people_.cend();
	people_.insert(iter, std::move(newPerson));
}

const std::vector<Person>& Container::RawVector() const
{
	return people_;
}



ProxyContainer::ProxyContainer(Container & container)
	: sorter_([](const Person& p1, const Person& p2)
{
	return p1 < p2;
}),
	container_(container),
	sorted_(sorter_(container_.RawVector()))
{}

void ProxyContainer::SetSorting(std::function<bool(const Person&, const Person&)>&& compareLess)
{
	//sortingCompareLess_ = std::move(compareLess);
	sorter_.SetSorting(std::move(compareLess));
	Sort();
}

bool ProxyContainer::IsDirty() const
{
	return dirty_;
}

size_t ProxyContainer::Size() const
{
	if (IsDirty())
		Sort();

	return container_.Size();
}

Person & ProxyContainer::Get(size_t indx)
{
	if (IsDirty())
		Sort();

	return container_.Get(indx);
}

const Person & ProxyContainer::Get(size_t indx) const
{
	if (IsDirty())
		Sort();

	return container_.Get(indx);
}

Person & ProxyContainer::operator[](size_t indx)
{
	return Get(indx);
}

const Person & ProxyContainer::operator[](size_t indx) const
{
	return Get(indx);
}

void ProxyContainer::Insert(Person && newPerson, size_t indx_before)
{
	dirty_ = true;
	container_.Insert(std::move(newPerson), indx_before);
}

void ProxyContainer::Sort() const
{
	sorter_(sorted_, container_.RawVector());
	dirty_ = false;
}

size_t ProxyContainer::TranslateIndx(size_t indx) const
{
	assert(indx < Size() && "invalid index");
	return sorted_[indx];
}
