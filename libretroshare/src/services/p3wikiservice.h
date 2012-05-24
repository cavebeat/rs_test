/*
 * libretroshare/src/services: p3wikiservice.h
 *
 * Wiki interface for RetroShare.
 *
 * Copyright 2012-2012 by Robert Fernie.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License Version 2 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.
 *
 * Please report all bugs and problems to "retroshare@lunamutt.com".
 *
 */

#ifndef P3_WIKI_SERVICE_HEADER
#define P3_WIKI_SERVICE_HEADER

#include "services/p3service.h"

#include "retroshare/rswiki.h"

#include <map>
#include <string>

/* 
 * Wiki Service
 *
 * This is an example service for the new cache system.
 * For the moment, it will only hold data passed to it from the GUI.
 * and spew that back when asked....
 *
 * We are doing it like this - so we can check the required interface functionality.
 *
 * Expect it won't take long before it'll be properly linked into the backend!
 *
 * This will be transformed into a Plugin Service, once the basics have been worked out.
 *
 */

class p3WikiService: public p3Service, public RsWiki
{
	public:

	p3WikiService(uint16_t type);

virtual int	tick();

	public:

// NEW INTERFACE.
/************* Extern Interface *******/

virtual bool updated();
virtual bool getGroupList(std::list<std::string> &group);

virtual bool getGroup(const std::string &groupid, RsWikiGroup &group);
virtual bool getPage(const std::string &pageid, RsWikiPage &page);
virtual bool getPageVersions(const std::string &origPageId, std::list<std::string> &pages);
virtual bool getOrigPageList(const std::string &groupid, std::list<std::string> &pageIds);
virtual bool getLatestPage(const std::string &origPageId, std::string &page);

virtual bool createGroup(RsWikiGroup &group);
virtual bool createPage(RsWikiPage &page);

	private:

std::string genRandomId();

	RsMutex mWikiMtx;

	/***** below here is locked *****/

	bool mUpdated;

	std::map<std::string, std::list<std::string > > mGroupToOrigPages;
	std::map<std::string, std::list<std::string > > mOrigToPageVersions;
	std::map<std::string, std::string> mOrigPageToLatestPage;
	std::map<std::string, RsWikiGroup> mGroups;
	std::map<std::string, RsWikiPage> mPages;

};

#endif 