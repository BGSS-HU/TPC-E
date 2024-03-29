/*
* Legal Notice
*
* This document and associated source code (the "Work") is a part of a
* benchmark specification maintained by the TPC.
*
* The TPC reserves all right, title, and interest to the Work as provided
* under U.S. and international laws, including without limitation all patent
* and trademark rights therein.
*
* No Warranty
*
* 1.1 TO THE MAXIMUM EXTENT PERMITTED BY APPLICABLE LAW, THE INFORMATION
*     CONTAINED HEREIN IS PROVIDED "AS IS" AND WITH ALL FAULTS, AND THE
*     AUTHORS AND DEVELOPERS OF THE WORK HEREBY DISCLAIM ALL OTHER
*     WARRANTIES AND CONDITIONS, EITHER EXPRESS, IMPLIED OR STATUTORY,
*     INCLUDING, BUT NOT LIMITED TO, ANY (IF ANY) IMPLIED WARRANTIES,
*     DUTIES OR CONDITIONS OF MERCHANTABILITY, OF FITNESS FOR A PARTICULAR
*     PURPOSE, OF ACCURACY OR COMPLETENESS OF RESPONSES, OF RESULTS, OF
*     WORKMANLIKE EFFORT, OF LACK OF VIRUSES, AND OF LACK OF NEGLIGENCE.
*     ALSO, THERE IS NO WARRANTY OR CONDITION OF TITLE, QUIET ENJOYMENT,
*     QUIET POSSESSION, CORRESPONDENCE TO DESCRIPTION OR NON-INFRINGEMENT
*     WITH REGARD TO THE WORK.
* 1.2 IN NO EVENT WILL ANY AUTHOR OR DEVELOPER OF THE WORK BE LIABLE TO
*     ANY OTHER PARTY FOR ANY DAMAGES, INCLUDING BUT NOT LIMITED TO THE
*     COST OF PROCURING SUBSTITUTE GOODS OR SERVICES, LOST PROFITS, LOSS
*     OF USE, LOSS OF DATA, OR ANY INCIDENTAL, CONSEQUENTIAL, DIRECT,
*     INDIRECT, OR SPECIAL DAMAGES WHETHER UNDER CONTRACT, TORT, WARRANTY,
*     OR OTHERWISE, ARISING IN ANY WAY OUT OF THIS OR ANY OTHER AGREEMENT
*     RELATING TO THE WORK, WHETHER OR NOT SUCH AUTHOR OR DEVELOPER HAD
*     ADVANCE NOTICE OF THE POSSIBILITY OF SUCH DAMAGES.
*
* Contributors
* - Doug Johnson
*/

#include "../inc/ExchangeDataFileRecord.h"

#include <iomanip>
#include <sstream>
#include <stdexcept>
#include <string>

#include "../inc/Utilities.h"

using namespace TPCE;

ExchangeDataFileRecord::ExchangeDataFileRecord(const std::deque<std::string>& fields)
{
    if (fieldCount != fields.size())
    {
        throw std::runtime_error("Incorrect field count.");
    }

    DFRStringInit( fields[0], ex_id, ex_idCStr, maxEx_idLen);

    DFRStringInit( fields[1], ex_name, ex_nameCStr, maxEx_nameLen);

    ex_open = std::stoi(fields[2]);
    ex_close = std::stoi(fields[3]);

    DFRStringInit( fields[4], ex_desc, ex_descCStr, maxEx_descLen);

    ex_ad_id = std::stoi(fields[5]);
}

const std::string& ExchangeDataFileRecord::EX_ID() const
{
    return ex_id;
}

const char* ExchangeDataFileRecord::EX_ID_CSTR() const
{
    return ex_idCStr;
}

const std::string& ExchangeDataFileRecord::EX_NAME() const
{
    return ex_name;
}

const char* ExchangeDataFileRecord::EX_NAME_CSTR() const
{
    return ex_nameCStr;
}

int ExchangeDataFileRecord::EX_OPEN() const
{
    return ex_open;
}

int ExchangeDataFileRecord::EX_CLOSE() const
{
    return ex_close;
}

const std::string& ExchangeDataFileRecord::EX_DESC() const
{
    return ex_desc;
}

const char* ExchangeDataFileRecord::EX_DESC_CSTR() const
{
    return ex_descCStr;
}

TIdent ExchangeDataFileRecord::EX_AD_ID() const
{
    return ex_ad_id;
}

std::string ExchangeDataFileRecord::ToString(char fieldSeparator) const
{
    // Facilitate encapsulation by using public interface to fields.
    std::ostringstream msg;
    msg << EX_ID() 
        << fieldSeparator << EX_NAME() 
        << fieldSeparator << std::setfill('0') << std::setw(4) << EX_OPEN() 
        << fieldSeparator << std::setfill('0') << std::setw(4) << EX_CLOSE() 
        << fieldSeparator << EX_DESC() 
        << fieldSeparator << EX_AD_ID();
    return msg.str();
}
