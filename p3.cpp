#include <iostream>
#include <string>

#include "p3.h"

using namespace std;

Region::Region() {
    this->name = "No Name Set";
    this->viewers = 0;
    this->next = nullptr;
}

Region::Region(std::string name, int viewers, Region *next) {
    this->name = name;
    this->viewers = viewers;
    this->next = next;
}

Media::Media() {
    this->name = "No Name Set";
    this->next = nullptr;
    this->headRegionList = nullptr;
}

Media::Media(std::string name, Media *next) {
    this->name = name;
    this->next = next;
    this->headRegionList = nullptr;
}

MediaList::MediaList() {
    this->headMediaList = nullptr;
    this->count = 0;
}

int MediaList::getCount() {
    return this->count;
}

void Region::print(std::ostream &os) {
    os << "    " << name << " | " << viewers << endl;
}

void Media::print(std::ostream &os) {
    os << "Media Name: " << name << endl;
    for (Region *region = headRegionList; region != nullptr; region = region->next){
        region->print(cout);
    }
}

// print the specified media and its regions
// 1. return false and print nothing if the media is not in the list
// 2. otherwise return true and print the media and its regions
bool MediaList::printMedia(std::ostream &os, std::string name) {
    Media *media = headMediaList;
    while (media != nullptr){
        if(media->name == name){
            media->print(cout);
            return true;
        }
        media = media->next;
    }
    return false;
}

// print all media and their regions
// print nothing if the list is empty
void MediaList::print(std::ostream &os) {
    Media *media = headMediaList;
    while (media != nullptr){
        media->print(cout);
        media = media->next;
    }
}

// add the media to the back of the list
// 1. return false and do nothing if the media is already in the list
// 2. otherwise return true and add the media to the back of the list
// - do not forget to update count
bool MediaList::addMedia(std::string name) {
    Media *media = headMediaList;
    Media *last = headMediaList;
    while (media != nullptr){
        if (media->name == name){
            return false;
        }
        if (media->next == nullptr){
            last = media;
        }
        media = media->next;
    }

    Media *newMedia = new Media(name, nullptr);
    if (last == nullptr){
        headMediaList = newMedia;
    }
    else{
         last->next = newMedia;
    }
    count++;

    return true;
}

// add the region to the back of the media's region list
// 1. return false and do nothing if the media is not in the list
// 2. if the regionName is already in the region list, update the viewers and return true
// 3. otherwise add the region to the back of the media's list and return true
bool MediaList::addRegion(std::string mediaName, std::string regionName, int viewers) {
    Media *media = headMediaList;
    while (media != nullptr){
        if (media->name == mediaName){
            Region *region = media->headRegionList;
            Region *last = media->headRegionList;
            while (region != nullptr){
                if (region->name == regionName){
                    region->viewers = viewers;
                    return true;
                }
                if (region->next == nullptr){
                    last = region;
                }
                region = region->next;
            }
            Region *newRegion = new Region(regionName, viewers, nullptr);
            if (last == nullptr){
                media->headRegionList = newRegion;
            }
            else {
                last->next = newRegion;
            }
            return true;
        }
        media = media->next;
    }
    return false;
}

// add the media to the list in ascending order by name
// 1. return false and do nothing if the media is already in the list
// 2. otherwise return true and add the media to the list
// - do not forget to update count
// - compare strings with the built-in comparison operators: <, >, ==, etc.
bool MediaList::addMediaOrdered(std::string name) {
    Media *media = headMediaList;
    Media *last = headMediaList;
    Media *next = headMediaList;
    while (media != nullptr){
        if (media->name == name){
            return false;
        }
        media = media->next;
    }
    
    media = headMediaList;

    Media *newMedia = new Media(name, nullptr);
    if (headMediaList == nullptr){
        headMediaList = newMedia;
        count++;
        return true;
    }
    if (name < headMediaList->name){
        newMedia->next = headMediaList;
        headMediaList = newMedia;
        count++;
        return true;
    }
    while (media->next != nullptr){
        last = media;
        next = media->next;
        if (name <= next->name){
            last->next = newMedia;
            newMedia->next = next;
            count++;
            return true;
        }
        media = media->next;
    }

    media->next = newMedia;
    count++;

    return true;
}

// add the region to the media's region list in descending order by viewers
// 1. return false and do nothing if the media is not in the list
// 2. if the regionName is already in the region list, update the viewers and return true
// 3. otherwise add the region to the media's list and return true
bool MediaList::addRegionOrdered(std::string mediaName, std::string regionName, int viewers) {
    Media *media = headMediaList;
    while (media != nullptr){
        if (media->name == mediaName){
            Region *region = media->headRegionList;
            Region *last = media->headRegionList;
            Region *next = media->headRegionList;
            while (region != nullptr){
                if (region->name == regionName){
                    region->viewers = viewers;
                    return true;
                }
                region = region->next;
            }

            region = media->headRegionList;

            Region *newRegion = new Region(regionName, viewers, nullptr);
            if (media->headRegionList == nullptr){
                media->headRegionList = newRegion;
                return true;
            }
            if (viewers >= media->headRegionList->viewers){
                newRegion->next = media->headRegionList;
                media->headRegionList = newRegion;
                return true;
            }
            while (region->next != nullptr){
                last = region;
                next = region->next;
                if (viewers >= next->viewers){
                    last->next = newRegion;
                    newRegion->next = next;
                    return true;
                }
                region = region->next;
            }

            region->next = newRegion;

            return true;
        }
        media = media->next;
    }
    return false;
}

// remove the media and its regions from the list
// 1. return false and do nothing if the media is not in the list
// 2. otherwise return true and remove the media and its regions
// - do not forget to update count
bool MediaList::removeMedia(std::string name) {
    Media *last = headMediaList;
    Media *media = headMediaList;
    Media *previous = headMediaList;
    if (media == nullptr){
        return false;
    }
    if (name == media->name){
        headMediaList = media->next;
        Region *region = media->headRegionList;
        while(region != nullptr){
            Region *lastRegion = region;
            region = region->next;

            delete lastRegion;
        }
        delete media;
        count--;
        return true;
    }
    while (last->next != nullptr){
        media = last->next;
        if (name == media->name){
            last->next = media->next;

            Region *region = media->headRegionList;
            while(region != nullptr){
                Region *lastRegion = region;
                region = region->next;

                delete lastRegion;
            }
            delete media;
            count--;
            return true;
        }
        previous = last;
        last = media;
    }
    if (name == last->name){
        previous->next = nullptr;

        Region *region = last->headRegionList;
        while(region != nullptr){
            Region *lastRegion = region;
            region = region->next;

            delete lastRegion;
        }
        delete last;
        count--;
        return true;
    }
    return false;
}

// remove the region from the media's region list
// 1. return false and do nothing if the media is not in the list
// 2. return false and do nothing if the region is not in the media's region list
// 3. otherwise return true and remove the region from the media's list
bool MediaList::removeRegion(std::string mediaName, std::string regionName) {
    Media *media = headMediaList;
    while (media != nullptr){
        if (media->name == mediaName){
            Region *region = media->headRegionList;
            Region *last = media->headRegionList;
            Region *previous = media->headRegionList;
            if (region == nullptr){
                return false;
            }
            if (regionName == region->name){
                media->headRegionList = region->next;

                delete region;
                return true;
            }
            while (last->next != nullptr){
                region = last->next;
                
                if (regionName == region->name){
                    last->next = region->next;
                    delete region;
                    return true;
                }
                previous = last;
                last = region;
            }
            if (regionName == last->name){
                previous->next = nullptr;
                delete last;
                return true;
            }
            return false;
        }
        media = media->next;
    }
    return false;
}

// destroy the list by removing all media and their regions
MediaList::~MediaList() {
    Media *media = headMediaList;
    Media *next = headMediaList;
    while (media != nullptr){
        next = media->next;
        Region *region = media->headRegionList;
        while(region != nullptr){
            Region *lastRegion = region;
            region = region->next;

            delete lastRegion;
        }
        delete media;

        media = next;
    }

}

// deep copy the source list
// - do not forget to update count
MediaList::MediaList(const MediaList &src) {
    this->count = src.count;
    Media *oldMedia = src.headMediaList;

    Media *head = new Media(oldMedia->name, nullptr);
    Region *oldRegion = oldMedia->headRegionList;
    head->headRegionList = new Region(oldRegion->name, oldRegion->viewers, nullptr);

    Region *newRegion = head->headRegionList;
    oldRegion = oldRegion->next;
    while(oldRegion != nullptr){
        Region *createdRegion = new Region(oldRegion->name, oldRegion->viewers, nullptr);

        newRegion->next = createdRegion;

        newRegion = createdRegion;
        oldRegion = oldRegion->next;

    }


    this->headMediaList = head;
    Media *newMedia = head;

    oldMedia = oldMedia->next;
    while(oldMedia != nullptr){
        Media *createdMedia = new Media(oldMedia->name, nullptr);

        oldRegion = oldMedia->headRegionList;
        createdMedia->headRegionList = new Region(oldRegion->name, oldRegion->viewers, nullptr);

        newRegion = createdMedia->headRegionList;
        oldRegion = oldRegion->next;
        while(oldRegion != nullptr){
            Region *createdRegion = new Region(oldRegion->name, oldRegion->viewers, nullptr);

            newRegion->next = createdRegion;

            newRegion = createdRegion;
            oldRegion = oldRegion->next;

        }


        newMedia->next = createdMedia;

        newMedia = createdMedia;
        oldMedia = oldMedia->next;
    }

}

// remove all media and their regions then deep copy the source list
// - do not forget to update count
const MediaList &MediaList::operator=(const MediaList &src) {
    if (this != &src) {
        Media *media = headMediaList;
        Media *next = headMediaList;
        while (media != nullptr){
            next = media->next;
            Region *region = media->headRegionList;
            while(region != nullptr){
                Region *lastRegion = region;
                region = region->next;

                delete lastRegion;
            }
            delete media;

            media = next;
        }
        this->count = src.count;
        Media *oldMedia = src.headMediaList;

        Media *head = new Media(oldMedia->name, nullptr);
        Region *oldRegion = oldMedia->headRegionList;
        head->headRegionList = new Region(oldRegion->name, oldRegion->viewers, nullptr);

        Region *newRegion = head->headRegionList;
        oldRegion = oldRegion->next;
        while(oldRegion != nullptr){
            Region *createdRegion = new Region(oldRegion->name, oldRegion->viewers, nullptr);

            newRegion->next = createdRegion;

            newRegion = createdRegion;
            oldRegion = oldRegion->next;

        }


        this->headMediaList = head;
        Media *newMedia = head;

        oldMedia = oldMedia->next;
        while(oldMedia != nullptr){
            Media *createdMedia = new Media(oldMedia->name, nullptr);

            oldRegion = oldMedia->headRegionList;
            createdMedia->headRegionList = new Region(oldRegion->name, oldRegion->viewers, nullptr);

            newRegion = createdMedia->headRegionList;
            oldRegion = oldRegion->next;
            while(oldRegion != nullptr){
                Region *createdRegion = new Region(oldRegion->name, oldRegion->viewers, nullptr);

                newRegion->next = createdRegion;

                newRegion = createdRegion;
                oldRegion = oldRegion->next;

            }


            newMedia->next = createdMedia;

            newMedia = createdMedia;
            oldMedia = oldMedia->next;
        }



    }
    return *this;
}
