/**
 * TODO: Add hat. */

#pragma once

#include <ATD/Core/Fs.hpp>

#include <map>
#include <memory>


namespace ATD {

/**
 * Class, derived from Loadable MUST store copies/ smart pointers to all the dependencies.
 * */
class Loadable
{
public:
	typedef std::map<Loadable *, std::shared_ptr<Fs::Path>> DependencyTable;


	Loadable();

	~Loadable();

	void Load(const Fs::Path &filename);

	void Save(const Fs::Path &filename) const;

	const DependencyTable &GetDependencyTable() const;

protected:
	/* Should be called from the constructor of the derived class. */
	void AddDependency(Loadable *dependency);

	/* Should be called from OnLoad(..) method of the derived class. */
	void SetDependencyPath(Loadable *dependency, const Fs::Path &filename);

	/* I use to call this inside OnSave() method. */
	const Fs::Path &GetDependencyPath(Loadable *dependency) const;

	/* Should be called every time, the derived class is modified. */
	void SetChanged();

	virtual void OnLoad(const Fs::Path &filename) = 0;

	virtual void OnLoadFinished();

	virtual void OnSave(const Fs::Path &filename) const = 0;

	virtual void OnSaveFinished() const;

private:
	void CheckDependencyTable() const;


	DependencyTable m_dependencyTable;
	mutable bool m_hasChanged;
};

} /* namespace ATD */
