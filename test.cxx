#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <chrono>
#include <stdexcept>

#include "FunctionRule.hxx"
#include "FunctionRelation.hxx"
#include "RecursiveFunctionRule.hxx"
#include "RecursiveFunctionManager.hxx"

/*
The following codes runs a simulation of the Lorentz Attractor using the FAlc header library.
*/

// First create instances of the Lorentz Attractor's differential equations. The parameters of the equation are stored in the instances
// ====================================================================================================================================

// Create instance of the function returning the dx/dt: dx/dt= sigma*(y-x)
//                                                      ==================
template<real T>
class XLorentz : public RecursiveFunctionRule<T>
{
public:
    XLorentz(const T x0, const T dt, const T sigma) : RecursiveFunctionRule<T>(x0), dt(dt), sigma(sigma) {};
    inline const T rule(FunctionRelation<T> **params, const unsigned int i) const override
    {
        return params[0]->getImageElem(i) + (dt*sigma*(params[1]->getImageElem(i) - params[0]->getImageElem(i)));
    }
private:
    const T dt, sigma;
};

// Create instance of the function returning the dy/dt: dy/dt= x*(ro-z) - y
//                                                      ===================
template<real T>
class YLorentz : public RecursiveFunctionRule<T>
{
public:
    YLorentz(const T x0, const T dt, const T ro) : RecursiveFunctionRule<T>(x0), dt(dt), ro(ro) {};
    inline const T rule(FunctionRelation<T> **params, const unsigned int i) const override
    {
        return params[1]->getImageElem(i) + (dt*(params[0]->getImageElem(i) * (ro-(params[2]->getImageElem(i))) - (params[1]->getImageElem(i))));
    }
private:
    const T dt, ro;
};

// Create instance of the function returning the dz/dt: dz/dt= xy - beta*z
//                                                      ==================
template<real T>
class ZLorentz : public RecursiveFunctionRule<T>
{
public:
    ZLorentz(const T x0, const T dt, const T beta) : RecursiveFunctionRule<T>(x0), dt(dt), beta(beta) {};
    inline const T rule(FunctionRelation<T> **params, const unsigned int i) const override
    {
        return params[2]->getImageElem(i) + (dt*(params[0]->getImageElem(i)*params[1]->getImageElem(i) - beta * params[2]->getImageElem(i)));
    }
private:
    const T dt, beta;
};


int main(int argc, char *argv[])
{

    // if an initial point x is specified the code runs a solver of the Lorentz Attractor given initial condition r(0) = (x,x,x)
    if (argc > 1)
    {
        const float x = std::stof(argv[1]);

        // initialize the Managers with a jagged array containing 1000 arrays storing 100 floats each
        RecursiveFunctionManager<float, XLorentz> X = RecursiveFunctionManager<float, XLorentz>(1000, 100, x, 0.001f, 10.0f); // First manager also creates the domain
        RecursiveFunctionManager<float, YLorentz> Y = RecursiveFunctionManager<float, YLorentz>(1000, 100, X.getDomain(), x, 0.001f, 28.0f); // second and third use the already created domain for memory efficiency
        RecursiveFunctionManager<float, ZLorentz> Z = RecursiveFunctionManager<float, ZLorentz>(1000, 100, X.getDomain(), x, 0.001f, 8.0f/3.0f);
        X.InitDomainAndX0(0.0f, 1000*100*0.001f); // initialize the domain and the inital condition
        Y.InitX0();                               // only intialize initial condition because domain has been initialized in previous object
        Z.InitX0();                               // only intialize initial condition because domain has been initialized in previous object

        // create an array of the relations (the managers will use them to update the system)
        FunctionRelation<float> *params[3] = {&X, &Y, &Z};

        // iteratively modify the domain
        for (int i=1;i<100000;i++)
        {
            X.modifyFromPrev(i, params);
            Y.modifyFromPrev(i, params);
            Z.modifyFromPrev(i, params);
        }
        
        // give the result of the run in a csv file.
        std::ofstream LorentzAttractor("lorentz_attractor.csv");
        for (int i=0;i<100000;i++)
        {
            LorentzAttractor << X.getImageElem(i) << "," << Y.getImageElem(i) << "," << Z.getImageElem(i) << "," << std::endl;
        }

        // If you want to run the simulation more in the same amount of memory run the following
        /*
        for (int j=0;i<5;i++)
            // modify the first element in array from last to restart the loop
            X.modifyFromLast(0, params);
            Y.modifyFromLast(0, params);
            Z.modifyFromLast(0, params);
            LorentzAttractor << X.getImageElem(0) << "," << Y.getImageElem(0) << "," << Z.getImageElem(0) << "," << std::endl;
            for (int i=1;i<100000;i++)
            {
                X.modifyFromPrev(i, params);
                Y.modifyFromPrev(i, params);
                Z.modifyFromPrev(i, params);
                // for example write the coordinates in itinere
                // ============================================
                LorentzAttractor << X.getImageElem(i) << "," << Y.getImageElem(i) << "," << Z.getImageElem(i) << "," << std::endl;
            }
            
        */

        // close the file
        LorentzAttractor.close();
        std::cout << "Creating file - DONE" << std::endl;

        return 1;
    }

    std::cout << "Running " << argv[0] << std::endl;
    return 0;
}