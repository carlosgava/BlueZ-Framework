//
//  Watcher.h
//  BlueZ-Framework
//
//  Created by Carlos Henrique Gava on 29/07/25.
//

#ifndef Watcher_h
#define Watcher_h

#include <iostream>
#include "Level.h"

namespace WatchDog {
    template<level Level = level::info, class Source = std::ostream>
    class Watcher {
    public:
        std::ostream& mouse;

        dog(Source &src = std::cerr) : mouse(src) {}

        template<class... Args>
        void yelp(Args &&... args) {
            bark<level::info>(std::forward<Args>(args)...);
        }

        template<class... Args>
        void bow(Args &&... args) {
            bark<level::warning>(std::forward<Args>(args)...);
        }

        template<class... Args>
        void woof(Args &&... args) {
            bark<level::error>(std::forward<Args>(args)...);
        }

    private:
        template<level L, class... Args>
        void bark(Args &&... args) {
            if(static_cast<int>(Level) <= static_cast<int>(L)){
                bark_impl(std::forward<Args>(args)...);
                mouse << std::endl;
            }
        }

        template<class Arg>
        void bark_impl(Arg && arg) {
            mouse << arg << ' ';
        }

        template<class Head, class... Tail>
        void bark_impl(Head && head, Tail &&... tail) {
            mouse << head << ' ';
            bark_impl(std::forward<Tail>(tail)...);
        }
    };

}

#endif /* Watcher_h */
