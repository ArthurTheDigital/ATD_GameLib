/**
 * @file      
 * @brief     Loadable interface.
 * @details   ...
 * @author    ArthurTheDigital (arthurthedigital@gmail.com)
 * @copyright GPL v3.
 * @since     $Id: $ */

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

	void load(const Fs::Path &filename);

	void save(const Fs::Path &filename) const;

	const DependencyTable &getDependencyTable() const;

protected:
	/* Should be called from the constructor of the derived class. */
	void addDependency(Loadable *dependency);

	/* Should be called from OnLoad(..) method of the derived class. */
	void setDependencyPath(Loadable *dependency, const Fs::Path &filename);

	/* I use to call this inside OnSave() method. */
	const Fs::Path &getDependencyPath(Loadable *dependency) const;

	/* Should be called every time, the derived class is modified. */
	void setChanged();

	virtual void onLoad(const Fs::Path &filename) = 0;

	virtual void onLoadFinished();

	virtual void onSave(const Fs::Path &filename) const = 0;

	virtual void onSaveFinished() const;

private:
	void checkDependencyTable() const;


	DependencyTable m_dependencyTable;
	mutable bool m_hasChanged;
};

} /* namespace ATD */
