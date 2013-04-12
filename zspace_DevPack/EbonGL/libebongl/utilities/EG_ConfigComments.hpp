/* DESCRIPTION
  Contains the comments for a single entry in the config file.
  Used by the ConfigWriter class.
*/

#ifndef EG_CONFIGCOMMENTS_HPP_INCLUDED
#define EG_CONFIGCOMMENTS_HPP_INCLUDED

#include <vector>
#include <string>

namespace EbonGL
{
    class EG_ConfigComments
    {
        public:
            //constructor
            EG_ConfigComments(void);

            //destructor
            ~EG_ConfigComments(void);

            //add a comment for this entry
            void add(std::string newComment);

            //returns the comment at the specified index
            std::string get(int index);

            //returns the number of comments being stored
            unsigned int size(void);

        private:
            std::vector <std::string> theComments;//the comments we're storing

    };//EG_ConfigComments
}//namespace EbonGL

#endif
