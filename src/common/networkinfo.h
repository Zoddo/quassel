/***************************************************************************
 *   Copyright (C) 2005-08 by the Quassel Project                          *
 *   devel@quassel-irc.org                                                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) version 3.                                           *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#ifndef _NETWORKINFO_H_
#define _NETWORKINFO_H_

#include <QString>
#include <QStringList>
#include <QList>
#include <QHash>
#include <QVariantMap>
#include <QPointer>

#include "types.h"
#include "syncableobject.h"

class SignalProxy;
class IrcUser;
class IrcChannel;


class NetworkInfo : public SyncableObject {
  Q_OBJECT

  Q_PROPERTY(QString networkName READ networkName WRITE setNetworkName STORED false)
  Q_PROPERTY(QString currentServer READ currentServer WRITE setCurrentServer STORED false)
  Q_PROPERTY(QString myNick READ myNick WRITE setMyNick STORED false)

public:
  NetworkInfo(const uint &networkid, QObject *parent = 0);
  //virtual ~NetworkInfo();

  NetworkId networkId() const;
  bool initialized() const;

  SignalProxy *proxy() const;
  void setProxy(SignalProxy *proxy);

  bool isMyNick(const QString &nick) const;
  bool isMyNick(IrcUser *ircuser) const;

  bool isChannelName(const QString &channelname) const;

  QString prefixToMode(const QString &prefix);
  QString prefixToMode(const QCharRef &prefix);
  QString modeToPrefix(const QString &mode);
  QString modeToPrefix(const QCharRef &mode);

  QString networkName() const;
  QString currentServer() const;
  QString myNick() const;
  QStringList nicks() const;
  QStringList channels() const;

  QString prefixes();
  QString prefixModes();

  bool supports(const QString &param) const;
  QString support(const QString &param) const;

  IrcUser *newIrcUser(const QString &hostmask);
  IrcUser *newIrcUser(const QByteArray &hostmask);
  IrcUser *ircUser(QString nickname) const;
  IrcUser *ircUser(const QByteArray &nickname) const;
  QList<IrcUser *> ircUsers() const;

  IrcChannel *newIrcChannel(const QString &channelname);
  IrcChannel *newIrcChannel(const QByteArray &channelname);
  IrcChannel *ircChannel(QString channelname);
  IrcChannel *ircChannel(const QByteArray &channelname);
  
  QList<IrcChannel *> ircChannels() const;

  QTextCodec *codecForEncoding() const;
  QTextCodec *codecForDecoding() const;
  void setCodecForEncoding(const QString &codecName);
  void setCodecForEncoding(QTextCodec *codec);
  void setCodecForDecoding(const QString &codecName);
  void setCodecForDecoding(QTextCodec *codec);

  QString decodeString(const QByteArray &text) const;
  QByteArray encodeString(const QString string) const;

public slots:
  void setNetworkName(const QString &networkName);
  void setCurrentServer(const QString &currentServer);
  void setMyNick(const QString &mynick);

  void addSupport(const QString &param, const QString &value = QString());
  void removeSupport(const QString &param);

  inline void addIrcUser(const QString &hostmask) { newIrcUser(hostmask); }
  void removeIrcUser(QString nick);
  
  //init geters
  QVariantMap initSupports() const;
  QStringList initIrcUsers() const;
  QStringList initIrcChannels() const;
  
  //init seters
  void initSetSupports(const QVariantMap &supports);
  void initSetIrcUsers(const QStringList &hostmasks);
  void initSetChannels(const QStringList &channels);
  
  IrcUser *updateNickFromMask(const QString &mask);

  // these slots are to keep the hashlists of all users and the
  // channel lists up to date
  void ircUserNickChanged(QString newnick);
  void setInitialized();

private slots:
  void ircUserDestroyed();
  void channelDestroyed();
  void removeIrcUser(IrcUser *ircuser);
  
signals:
  void networkNameSet(const QString &networkName);
  void currentServerSet(const QString &currentServer);
  void myNickSet(const QString &mynick);

  void supportAdded(const QString &param, const QString &value);
  void supportRemoved(const QString &param);
  
  void ircUserAdded(QString hostmask);
  void ircChannelAdded(QString channelname);

  void ircUserRemoved(QString nick);
  
  void initDone();
  void ircUserInitDone();
  void ircChannelInitDone();
  
private:
  uint _networkId;
  bool _initialized;
  
  QString _myNick;
  QString _networkName;
  QString _currentServer;

  QString _prefixes;
  QString _prefixModes;

  QHash<QString, IrcUser *> _ircUsers;  // stores all known nicks for the server
  QHash<QString, IrcChannel *> _ircChannels; // stores all known channels
  QHash<QString, QString> _supports;  // stores results from RPL_ISUPPORT

  //QVariantMap networkSettings;
  //QVariantMap identity;
  
  QPointer<SignalProxy> _proxy;
  void determinePrefixes();

  QTextCodec *_codecForEncoding;
  QTextCodec *_codecForDecoding;

};

#endif
