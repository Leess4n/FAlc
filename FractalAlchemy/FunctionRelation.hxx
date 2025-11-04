#pragma once

#include <concepts>

#include "FunctionRule.hxx"
#include "RecursiveFunctionRule.hxx"

template<real T>
class RecursiveFunctionRule;


/*
Function relation class. It stores the elements of the domain and the corresponding image
mimicking the behaviour of the mathematical concept of relation. For a function this means
relating an element from the domain to an element in the codomain.

This class stores domain and codomain as jagged arrays to provide a flexible system in case
of large arrays
*/
template<real T>
class FunctionRelation
{
public:
    // Ctors
    // =====
    FunctionRelation(size_t I, size_t J);             // Initializes a domain and image by allocating necessary memory. I*J is the size of the domain and image
    FunctionRelation(size_t I, size_t J, T **domain); // Initializes a domain and image by using a pointer to another FunctionRelation's domain
    ~FunctionRelation();                              // Default ctor

    // populate whole domain and image
    void populateDomainFromInterval(const T x0, const T x1);                                            // evenly spaces out points in the domain from a given interval 
    void populateImageFromSimpleFunc(FunctionRule<T> &f);                                               // given a FunctionRule object creates an image calling the operator() of the function
    void populateImageFromSingleRecursiveFunc(RecursiveFunctionRule<T> &f, FunctionRelation<T> **func); // creates an image from a recursive function
    // modify image
    void modifyFromSimpleFunc(FunctionRule<T> &f, const unsigned int i);                                                            // modifies the image using the rule of `f` to the element of index `i` in the domain
    inline void modifyFromPrevRecursiveFunc(const RecursiveFunctionRule<T> &f, const unsigned int i, FunctionRelation<T> **params); // modifies an element in the image at index `i` using rule `f` on the previous value
    inline void modifyFromLastRecursiveFunc(const RecursiveFunctionRule<T> &f, const unsigned int i, FunctionRelation<T> **params); // modifies the last element in the image at index `i` using rule `f` on the previous value
 
    // getter functions
    inline const T getDomainElem(const unsigned int i) const;
    inline const T getImageElem(const unsigned int i) const;
    inline T** getDomain() const;
    inline T** getImage() const;
    inline const bool isDomainAssigned() const;
    inline const bool isDomainAllocated() const;
    inline const T getdx() const;
    inline const size_t getI() const;
    inline const size_t getJ() const;

    // setter functions
    inline void setDomainElem(const T x, const unsigned int i);
    inline void setImageElem(const T x, const unsigned int i);

private:
    // domain and image are jagged arrays of size I containing arrays of size J
    T **domain;
    T **image;

    // dimesions of arrays
    size_t I, J;
    // distances between consecutive elements in domain
    T dx;
    // variables for safety
    bool domainAssigned = false, domainAllocated = false, imageAssigned = false;
};

#include "FunctionRelation_impl.hxx"