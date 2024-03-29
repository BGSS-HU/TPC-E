#ifndef WEIGHTED_DATA_FILE_H
#define WEIGHTED_DATA_FILE_H

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

#include <deque>
#include <sstream>
#include <vector>
#include <stdexcept>

#include <string>

#include "ITextSplitter.h"

namespace TPCE
{
    //
    // Description:
    // A template class for converting a series of weighted text records
    // into a binary in-memory structure for quick easy access.
    //
    // Exception Safety:
    // The Basic guarantee is provided.
    //
    // Copy Behavior:
    // Copying is allowed.
    //
    template<class RecordType>
    class WeightedDataFile
    {
    private:
        typedef std::vector<RecordType> Records; // For convenience and readability
        Records records;

        typedef std::vector<int> Weights; // For convenience and readability
        Weights weightedIndexes;

    public:
        // Leverage the size type of our underlying storage container but
        // insulate clients from the implementation particulars by creating
        // our own type.
        typedef typename Records::size_type size_type;

        enum SizeFilter { AllRecords, UniqueRecordsOnly };

        explicit WeightedDataFile(ITextSplitter& splitter)
        {
            // eof only returns true after trying to read the end, so
            // "prime the pump" by doing an initial read.
            std::deque<std::string> fields = splitter.getNextRecord();

            //Process each record.
            while (!splitter.eof())
            {
                if (1 == fields.size() && "" == fields[0])
                {
                    // We found a blank line so skip it and move on.
                    fields = splitter.getNextRecord();
                    continue;
                }

                // The first field is the weight for this record.
                int weight = std::stoi(fields[0]);
                fields.pop_front();

                // Set up the weighted indexes for the record.
                for (int ii = 0; ii < weight; ++ii)
                {
                    weightedIndexes.push_back(records.size());
                }

                // Add the record.
                records.push_back(RecordType(fields));

                // Move on to the next record.
                fields = splitter.getNextRecord();
            }

            // Now that everything has been loaded tighten up our storage.
            weightedIndexes.shrink_to_fit();
            records.shrink_to_fit();
        }

        //
        // Default copies and destructor are ok.
        //
        // ~WeightedDataFile();
        // WeightedDataFile(const WeightedDataFile&);
        // WeightedDataFile& operator=(const WeightedDataFile&);
        //

        size_type size(SizeFilter filter = AllRecords) const
        {
            return( filter==AllRecords ? weightedIndexes.size() : records.size() );
        }

        const RecordType& operator[] (size_type weightedIndex) const
        {
            return records[weightedIndexes[weightedIndex]];
        }

        const RecordType& at(size_type weightedIndex) const
        {
            return records.at(weightedIndexes.at(weightedIndex));
        }

        const RecordType& getUniqueRecord(size_type idx, bool rangeCheckedAccess = false) const
        {
            return ( rangeCheckedAccess ? records.at(idx) : records[idx] );
        }
    };

}   // namespace TPCE
#endif // WEIGHTED_DATA_FILE_H
