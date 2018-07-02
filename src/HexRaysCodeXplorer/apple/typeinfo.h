/*
 * Copyright (c) 2010 Apple Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */
#ifndef X_TYPEINFO
#define X_TYPEINFO

#include <exception>
#include <stddef.h>

namespace x__cxxabiv1 {
class __class_type_info;
}

namespace xstd {

class type_info {
    type_info &operator=(const type_info &);
    type_info(const type_info &);

  protected:
    const char *__type_name;

    explicit type_info(const char *__n) : __type_name(__n) {}

  public:
    virtual ~type_info();

    const char *name() const { return __type_name; }

    bool before(const type_info &__arg) const { return __type_name < __arg.__type_name; }
    size_t hash_code() const throw() { return *reinterpret_cast<const size_t *>(&__type_name); }

    bool operator==(const type_info &__arg) const { return __type_name == __arg.__type_name; }
    bool operator!=(const type_info &__arg) const { return !operator==(__arg); }

    virtual bool __is_pointer_p() const;
    virtual bool __is_function_p() const;
    virtual bool __do_catch(const type_info *, void **, unsigned) const;
    virtual bool __do_upcast(const x__cxxabiv1::__class_type_info *, void **) const;
};

class bad_cast : public std::exception {
  public:
    bad_cast() throw() {}
    virtual ~bad_cast() throw();
    virtual const char *what() const throw();
};

class bad_typeid : public std::exception {
  public:
    bad_typeid() throw() {}
    virtual ~bad_typeid() throw();
    virtual const char *what() const throw();
};

} // namespace xstd

#endif // _TYPEINFO
