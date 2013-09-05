/*
 * TemplateClass.h
 *
 *  Created on: 2013-8-28
 *      Author: ghdawn
 */

#ifndef TEMPLATECLASS_H_
#define TEMPLATECLASS_H_

namespace Temp
{
    /**
     * ...
     */
    typedef struct
    {
            int key;
            int score;
            Tree left;
            Tree right;
    }Node,*Tree;

    /**
     * a temp
     */
    class AClass
    {
        public:
            const double pi=3.14;
            AClass();
            virtual ~AClass();

            /**
             * get ID
             * @return id
             */
            inline int getId() const
            {
                return id;
            }

            /**
             *set ID
             * @param id
             */
            inline void setId(int id)
            {
                this->id = id;
            }

            /**
             * get name
             * @return name
             */
            char getName() const;

            /**
             *
             * @param name
             */
            void setName(char name);

            /**
             * copy from another one
             * @param temp another one
             */
            void copy(const AClass & temp);

            int length();
        private:
            int id;
            char name;
    };

} /* namespace Temp */
#endif /* TEMPLATECLASS_H_ */
