/****************************************************************
 *  RetroShare is distributed under the following license:
 *
 *  Copyright (C) 2008 Robert Fernie
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 *  Boston, MA  02110-1301, USA.
 ****************************************************************/

#ifndef _GXSFORUMSDIALOG_H
#define _GXSFORUMSDIALOG_H

#include <QThread>

#include "mainpage.h"
#include "RsAutoUpdatePage.h"
#include "ui_GxsForumsDialog.h"

#include <inttypes.h>

#include "util/TokenQueue.h"

#include <retroshare/rsgxsforums.h>

#include "gui/gxs/GxsIdTreeWidgetItem.h"

class ForumInfo;


/* These are all the parameters that are required for thread loading. 
 * They are kept static for the load duration.
 */

class GxsForumsThreadLoadParameters
{
public:
	std::string ForumId;
	std::string FocusMsgId;

	uint32_t SubscribeFlags;
	int ViewType;
	uint32_t FilterColumn;

	std::map<uint32_t, QTreeWidgetItem *> MsgTokens;
	QList<QTreeWidgetItem*> Items;
	QList<QTreeWidgetItem*> ItemToExpand;

	bool FillComplete;
	bool FlatView;
	bool UseChildTS;
	bool ExpandNewMessages;
};

class GxsForumsDialog : public RsAutoUpdatePage, public TokenResponse 
{
	Q_OBJECT

	Q_PROPERTY(QColor textColorRead READ textColorRead WRITE setTextColorRead)
	Q_PROPERTY(QColor textColorUnread READ textColorUnread WRITE setTextColorUnread)
	Q_PROPERTY(QColor textColorUnreadChildren READ textColorUnreadChildren WRITE setTextColorUnreadChildren)
	Q_PROPERTY(QColor textColorNotSubscribed READ textColorNotSubscribed WRITE setTextColorNotSubscribed)
	Q_PROPERTY(QColor textColorMissing READ textColorMissing WRITE setTextColorMissing)

public:
	GxsForumsDialog(QWidget *parent = 0);
	~GxsForumsDialog();

//#AFTER MERGE	virtual UserNotify *getUserNotify(QObject *parent);

	bool navigate(const std::string& forumId, const std::string& msgId);

	/* overloaded from RsAuthUpdatePage */
	virtual void updateDisplay();

	// Callback for all Loads.
	virtual void loadRequest(const TokenQueue *queue, const TokenRequest &req);

	QColor textColorRead() const { return mTextColorRead; }
	QColor textColorUnread() const { return mTextColorUnread; }
	QColor textColorUnreadChildren() const { return mTextColorUnreadChildren; }
	QColor textColorNotSubscribed() const { return mTextColorNotSubscribed; }
	QColor textColorMissing() const { return mTextColorMissing; }

	void setTextColorRead(QColor color) { mTextColorRead = color; }
	void setTextColorUnread(QColor color) { mTextColorUnread = color; }
	void setTextColorUnreadChildren(QColor color) { mTextColorUnreadChildren = color; }
	void setTextColorNotSubscribed(QColor color) { mTextColorNotSubscribed = color; }
	void setTextColorMissing(QColor color) { mTextColorMissing = color; }

protected:
	bool eventFilter(QObject *obj, QEvent *ev);
	void changeEvent(QEvent *e);

private slots:
	void forceUpdateDisplay(); // TEMP HACK FN.

	/** Create the context popup menu and it's submenus */
	void forumListCustomPopupMenu( QPoint point );
	void threadListCustomPopupMenu( QPoint point );
	void restoreForumKeys();
	void newforum();

	void changedForum(const QString &id);
	void changedThread();
	void clickedThread (QTreeWidgetItem *item, int column);

	void replytomessage();
	void replyMessageData(const RsGxsForumMsg &msg);

	//void print();
	//void printpreview();

	//void removemessage();
	void markMsgAsRead();
	void markMsgAsReadChildren();
	void markMsgAsReadAll();
	void markMsgAsUnread();
	void markMsgAsUnreadAll();
	void markMsgAsUnreadChildren();
	void copyForumLink();
	void copyMessageLink();

	/* handle splitter */
	void togglethreadview();

	void createthread();
	void createmessage();

	void subscribeToForum();
	void unsubscribeToForum();

	void showForumDetails();
	void editForumDetails();

	void previousMessage ();
	void nextMessage ();
	void nextUnreadMessage();
	void downloadAllFiles();

	void changedViewBox();

	void filterColumnChanged(int column);
	void filterItems(const QString &text);

	void generateMassData();

	void fillThreadFinished();
	void fillThreadProgress(int current, int count);

	void shareKey();

private:
	void insertForums();
	void insertThreads();
	void insertPost();
	void insertPostData(const RsGxsForumMsg &msg); // Second Half.

	// Utility Fns.
	QString titleFromInfo(const RsMsgMetaData &meta);
	QString messageFromInfo(const RsGxsForumMsg &msg);
	
	void forumMsgReadStatusChanged(const QString &forumId, const QString &msgId, int status);
	
	void updateMessageSummaryList(std::string forumId);
//	void forumInfoToGroupItemInfo(const ForumInfo &forumInfo, GroupItemInfo &groupItemInfo);
	void forumInfoToGroupItemInfo(const RsGroupMetaData &forumInfo, GroupItemInfo &groupItemInfo);

	void forumSubscribe(bool subscribe);
	void FillThreads(QList<QTreeWidgetItem *> &ThreadList, bool bExpandNewMessages, QList<QTreeWidgetItem*> &itemToExpand);
	void FillChildren(QTreeWidgetItem *Parent, QTreeWidgetItem *NewParent, bool bExpandNewMessages, QList<QTreeWidgetItem*> &itemToExpand);

	int getSelectedMsgCount(QList<QTreeWidgetItem*> *pRows, QList<QTreeWidgetItem*> *pRowsRead, QList<QTreeWidgetItem*> *pRowsUnread);
	void setMsgReadStatus(QList<QTreeWidgetItem*> &Rows, bool bRead);
	void markMsgAsReadUnread(bool bRead, bool bChildren, bool bForum);
	void CalculateIconsAndFonts(QTreeWidgetItem *pItem = NULL);
	void CalculateIconsAndFonts(QTreeWidgetItem *pItem, bool &bHasReadChilddren, bool &bHasUnreadChilddren);

	void processSettings(bool bLoad);
	void togglethreadview_internal();

	bool filterItem(QTreeWidgetItem *pItem, const QString &text, int filterColumn);

	// New Request/Response Loading Functions.
	void insertForumsData(const std::list<RsGroupMetaData> &forumList);
	void insertForumThreads(const RsGroupMetaData &fi);

	void requestGroupSummary();
	void loadGroupSummary(const uint32_t &token);

	void requestGroupSummary_CurrentForum(const std::string &forumId);
	void loadGroupSummary_CurrentForum(const uint32_t &token);

	void loadCurrentForumThreads(const std::string &forumId);
	void requestGroupThreadData_InsertThreads(const std::string &forumId);
	void loadGroupThreadData_InsertThreads(const uint32_t &token);
	void loadForumBaseThread(const RsGxsForumMsg &msg);

	void requestChildData_InsertThreads(uint32_t &token, const RsGxsGrpMsgIdPair &parentId);
	void loadChildData_InsertThreads(const uint32_t &token);
	void loadForumChildMsg(const RsGxsForumMsg &msg, QTreeWidgetItem *parent);

	void requestMsgData_InsertPost(const RsGxsGrpMsgIdPair &msgId);
	void loadMsgData_InsertPost(const uint32_t &token);
	void requestMsgData_ReplyMessage(const RsGxsGrpMsgIdPair &msgId);
	void loadMsgData_ReplyMessage(const uint32_t &token);

	bool convertMsgToThreadWidget(const RsGxsForumMsg &msgInfo, bool useChildTS, uint32_t filterColumn, GxsIdTreeWidgetItem *item);
//	bool convertMsgToThreadWidget(const RsGxsForumMsg &msgInfo, std::string authorName, bool useChildTS, uint32_t filterColumn, QTreeWidgetItem *item);

	TokenQueue *mForumQueue;

	bool m_bProcessSettings;
	bool inMsgAsReadUnread;

	QTreeWidgetItem *yourForums;
	QTreeWidgetItem *subscribedForums;
	QTreeWidgetItem *popularForums;
	QTreeWidgetItem *otherForums;

	RSTreeWidgetItemCompareRole *threadCompareRole;
	std::string mCurrForumId;
	std::string mCurrThreadId;
	int subscribeFlags;

	int lastViewType;
	std::string lastForumID;

	// New Datatypes to replace the FillThread.
	bool mThreadLoading;
	GxsForumsThreadLoadParameters mThreadLoad;

	/* Color definitions (for standard see qss.default) */
	QColor mTextColorRead;
	QColor mTextColorUnread;
	QColor mTextColorUnreadChildren;
	QColor mTextColorNotSubscribed;
	QColor mTextColorMissing;

	/** Qt Designer generated object */
	Ui::GxsForumsDialog ui;
};

#endif