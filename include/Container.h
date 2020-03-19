#pragma once

#include <string>
#include <vector>
#include <functional>
#include <type_traits>

// Holds person data. Not thread-safe
class Person
{
	std::string name_,
		surname_;
	size_t id_;

public:

	Person(const char *name, const char *surname, size_t id);
	Person(const std::string& name, const std::string& surname, size_t id);

	const std::string& Name() const;
	const std::string& Surname() const;
	size_t ID() const;

	void SetName(const char *name);
	void SetName(const std::string& name);
	void SetSurname(const char *surname);
	void SetSurname(const std::string& surname);
	void SetID(size_t id);

	// default compare less: surname, name, id
	bool operator<(const Person& other) const;

};

// holds records of people, not thread-safe
class Container
{
	std::vector<Person> people_;

public:

	template <class ... T>
	Container(T&& ... people)
		: people_({ std::forward<T>(people)... })
	{
		static_assert(std::conjunction_v<std::is_same<Person, T>...> && 
			"Only objects of type Person are allowed!");
	}

	size_t Size() const;

	Person& Get(size_t indx);
	const Person& Get(size_t indx) const;
	Person& operator[](size_t indx);
	const Person& operator[](size_t indx) const;

	// insert new Person. By default - to the end
	void Insert(Person&& newPerson, 
		size_t indx_before = std::numeric_limits<size_t>::max());

	// workaround for ProxySorter
	const std::vector<Person>& RawVector() const;
};

// creates proxy vector of sorted indices
template <typename T>
class ProxySorter
{
	std::function<bool(const T&, const T&)> compareLess_;

public:
	ProxySorter(std::function<bool(const T&, const T&)>&& compareLess)
		: compareLess_(std::move(compareLess))
	{
		assert(compareLess_ && "Function is empty!");
		if (!compareLess_)
			throw std::invalid_argument("Initialization using empty function");
	}

	void SetSorting(std::function<bool(const T&, const T&)>&& compareLess)
	{
		assert(compareLess && "Setting empty function");
		compareLess_ = std::move(compareLess);
	}

	static void Sort(std::vector<size_t> &proxyVec, const std::vector<T>& vec,
		const std::function<bool(const T&, const T&)>& compareLess)
	{
		using ProxyIndx = std::pair<const T*, size_t>;

		proxyVec.resize(vec.size());

		std::vector<ProxyIndx> sortIndices( vec.size() );
		for (size_t i = 0; i != vec.size(); ++i)
			sortIndices[i] = std::make_pair(&vec[i], i);

		std::sort(sortIndices.begin(), sortIndices.end(),
			[&](const ProxyIndx &pi1, const ProxyIndx &pi2)
		{
			const T &t1 = *pi1.first,
				&t2 = *pi2.first;
			return compareLess(t1, t2);
		});

		for (size_t i = 0; i != proxyVec.size(); ++i)
		{
			proxyVec[i] = sortIndices[i].second;
		}
	}

	void Sort(std::vector<size_t> &proxyVec, const std::vector<T>& vec) const
	{
		return Sort(proxyVec, vec, compareLess_);
	}

	void operator()(std::vector<size_t> &proxyVec, const std::vector<T>& vec) const
	{
		return Sort(proxyVec, vec);
	}

	static std::vector<size_t> Sort(const std::vector<T>& vec,
		const std::function<bool(const T&, const T&)>& compareLess)
	{
		std::vector<size_t>	outIndices( vec.size() );

		Sort(outIndices, vec, compareLess);

		return outIndices;
	}

	std::vector<size_t> Sort(const std::vector<T>& vec) const
	{
		return Sort(vec, compareLess_);
	}

	std::vector<size_t> operator()(const std::vector<T>& vec) const
	{
		return Sort(vec, compareLess_);
	}

};

constexpr size_t a = sizeof(Container);

// proxy container to provide custom sorting
class ProxyContainer
{
	ProxySorter<Person> sorter_;
	Container &container_;
	mutable std::vector<size_t> sorted_;
	//std::function<bool(const Person&, const Person&)> sortingCompareLess_;
	mutable bool dirty_;

public:

	ProxyContainer(Container &container);

	void SetSorting(std::function<bool(const Person&, const Person&)>&& compareLess);
	bool IsDirty() const;

	size_t Size() const;

	Person& Get(size_t indx);
	const Person& Get(size_t indx) const;
	Person& operator[](size_t indx);
	const Person& operator[](size_t indx) const;

	// insert new Person. By default - to the end
	void Insert(Person&& newPerson, 
		size_t indx_before = std::numeric_limits<size_t>::max());

	void Sort() const;

private:

	size_t TranslateIndx(size_t indx) const;

};
