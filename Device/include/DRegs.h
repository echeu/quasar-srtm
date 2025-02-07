
/*  �� Copyright CERN, 2015. All rights not expressly granted are reserved.

    The stub of this file was generated by quasar (https://github.com/quasar-team/quasar/)

    Quasar is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public Licence as published by
    the Free Software Foundation, either version 3 of the Licence.
    Quasar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public Licence for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Quasar.  If not, see <http://www.gnu.org/licenses/>.


 */


#ifndef __DRegs__H__
#define __DRegs__H__

#include <Base_DRegs.h>
#include "cJSON.h"

namespace Device
{

class
    DRegs
    : public Base_DRegs
{

public:
    /* sample constructor */
    explicit DRegs (
        const Configuration::Regs& config,
        Parent_DRegs* parent
    ) ;
    /* sample dtr */
    ~DRegs ();

    /* delegators for
    cachevariables and sourcevariables */


    /* delegators for methods */

private:
    /* Delete copy constructor and assignment operator */
    DRegs( const DRegs& other );
    DRegs& operator=(const DRegs& other);

    // ----------------------------------------------------------------------- *
    // -     CUSTOM CODE STARTS BELOW THIS COMMENT.                            *
    // -     Don't change this comment, otherwise merge tool may be troubled.  *
    // ----------------------------------------------------------------------- *

public:
    void update();
    void extract_ltc(cJSON *);
    void extract_firefly(cJSON *);
    void extract_fpga(cJSON *);
    //    void disableFF11();
    //    void enableFF11();

private:



};

}

#endif // __DRegs__H__
