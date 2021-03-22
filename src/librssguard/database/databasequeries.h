// For license of this file, see <project-root-folder>/LICENSE.md.

#ifndef DATABASEQUERIES_H
#define DATABASEQUERIES_H

#include "services/abstract/rootitem.h"

#include "core/messagefilter.h"
#include "definitions/typedefs.h"
#include "miscellaneous/application.h"
#include "miscellaneous/iconfactory.h"
#include "miscellaneous/textfactory.h"
#include "services/abstract/category.h"
#include "services/abstract/label.h"
#include "services/abstract/serviceroot.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QMultiMap>
#include <QSqlError>
#include <QSqlQuery>

class DatabaseQueries {
  public:
    static QMap<int, QString> messageTableAttributes(bool only_msg_table);

    // Custom data serializers.
    static QString serializeCustomData(const QVariantHash& data);
    static QVariantHash deserializeCustomData(const QString& data);

    // Label operators.
    static bool isLabelAssignedToMessage(const QSqlDatabase& db, Label* label, const Message& msg);
    static bool deassignLabelFromMessage(const QSqlDatabase& db, Label* label, const Message& msg);
    static bool assignLabelToMessage(const QSqlDatabase& db, Label* label, const Message& msg);
    static bool setLabelsForMessage(const QSqlDatabase& db, const QList<Label*>& labels, const Message& msg);
    static QList<Label*> getLabelsForAccount(const QSqlDatabase& db, int account_id);
    static QList<Label*> getLabelsForMessage(const QSqlDatabase& db, const Message& msg, const QList<Label*> installed_labels);
    static bool updateLabel(const QSqlDatabase& db, Label* label);
    static bool deleteLabel(const QSqlDatabase& db, Label* label);
    static bool createLabel(const QSqlDatabase& db, Label* label, int account_id);

    // Message operators.
    static bool markLabelledMessagesReadUnread(const QSqlDatabase& db, Label* label, RootItem::ReadStatus read);
    static bool markImportantMessagesReadUnread(const QSqlDatabase& db, int account_id, RootItem::ReadStatus read);
    static bool markMessagesReadUnread(const QSqlDatabase& db, const QStringList& ids, RootItem::ReadStatus read);
    static bool markMessageImportant(const QSqlDatabase& db, int id, RootItem::Importance importance);
    static bool markFeedsReadUnread(const QSqlDatabase& db, const QStringList& ids, int account_id, RootItem::ReadStatus read);
    static bool markBinReadUnread(const QSqlDatabase& db, int account_id, RootItem::ReadStatus read);
    static bool markAccountReadUnread(const QSqlDatabase& db, int account_id, RootItem::ReadStatus read);
    static bool switchMessagesImportance(const QSqlDatabase& db, const QStringList& ids);
    static bool permanentlyDeleteMessages(const QSqlDatabase& db, const QStringList& ids);
    static bool deleteOrRestoreMessagesToFromBin(const QSqlDatabase& db, const QStringList& ids, bool deleted);
    static bool restoreBin(const QSqlDatabase& db, int account_id);

    // Purge database.
    static bool purgeMessage(const QSqlDatabase& db, int message_id);
    static bool purgeImportantMessages(const QSqlDatabase& db);
    static bool purgeReadMessages(const QSqlDatabase& db);
    static bool purgeOldMessages(const QSqlDatabase& db, int older_than_days);
    static bool purgeRecycleBin(const QSqlDatabase& db);
    static bool purgeMessagesFromBin(const QSqlDatabase& db, bool clear_only_read, int account_id);
    static bool purgeLeftoverMessages(const QSqlDatabase& db, int account_id);

    // Purges message/label assignments where source message or label does not exist.
    // If account ID smaller than 1 is passed, then do this for all accounts.
    static bool purgeLeftoverLabelAssignments(const QSqlDatabase& db, int account_id = 0);
    static bool purgeLabelsAndLabelAssignments(const QSqlDatabase& db, int account_id);

    // Counts of unread/all messages.
    static QMap<QString, QPair<int, int>> getMessageCountsForCategory(const QSqlDatabase& db, const QString& custom_id,
                                                                      int account_id, bool only_total_counts,
                                                                      bool* ok = nullptr);
    static QMap<QString, QPair<int, int>> getMessageCountsForAccount(const QSqlDatabase& db, int account_id,
                                                                     bool only_total_counts, bool* ok = nullptr);
    static int getMessageCountsForFeed(const QSqlDatabase& db, const QString& feed_custom_id, int account_id,
                                       bool only_total_counts, bool* ok = nullptr);
    static int getMessageCountsForLabel(const QSqlDatabase& db, Label* label, int account_id,
                                        bool only_total_counts, bool* ok = nullptr);
    static int getImportantMessageCounts(const QSqlDatabase& db, int account_id,
                                         bool only_total_counts, bool* ok = nullptr);
    static int getMessageCountsForBin(const QSqlDatabase& db, int account_id, bool including_total_counts, bool* ok = nullptr);

    // Get messages (for newspaper view for example).
    static QList<Message> getUndeletedMessagesWithLabel(const QSqlDatabase& db, const Label* label, bool* ok = nullptr);
    static QList<Message> getUndeletedLabelledMessages(const QSqlDatabase& db, int account_id, bool* ok = nullptr);
    static QList<Message> getUndeletedImportantMessages(const QSqlDatabase& db, int account_id, bool* ok = nullptr);
    static QList<Message> getUndeletedMessagesForFeed(const QSqlDatabase& db, const QString& feed_custom_id,
                                                      int account_id, bool* ok = nullptr);
    static QList<Message> getUndeletedMessagesForBin(const QSqlDatabase& db, int account_id, bool* ok = nullptr);
    static QList<Message> getUndeletedMessagesForAccount(const QSqlDatabase& db, int account_id, bool* ok = nullptr);

    // Custom ID accumulators.
    static QStringList customIdsOfMessagesFromLabel(const QSqlDatabase& db, Label* label, bool* ok = nullptr);
    static QStringList customIdsOfImportantMessages(const QSqlDatabase& db, int account_id, bool* ok = nullptr);
    static QStringList customIdsOfMessagesFromAccount(const QSqlDatabase& db, int account_id, bool* ok = nullptr);
    static QStringList customIdsOfMessagesFromBin(const QSqlDatabase& db, int account_id, bool* ok = nullptr);
    static QStringList customIdsOfMessagesFromFeed(const QSqlDatabase& db, const QString& feed_custom_id, int account_id,
                                                   bool* ok = nullptr);

    // Common account methods.
    template<typename T>
    static QList<ServiceRoot*> getAccounts(const QSqlDatabase& db, const QString& code, bool* ok = nullptr);

    template<typename Categ, typename Fee>
    static void loadFromDatabase(ServiceRoot* root);
    static bool storeNewOauthTokens(const QSqlDatabase& db, const QString& refresh_token, int account_id);
    static void createOverwriteAccount(const QSqlDatabase& db, ServiceRoot* account);
    static int updateMessages(QSqlDatabase db, const QList<Message>& messages, const QString& feed_custom_id,
                              int account_id, const QString& url, bool force_update, bool* any_message_changed, bool* ok = nullptr);
    static bool deleteAccount(const QSqlDatabase& db, int account_id);
    static bool deleteAccountData(const QSqlDatabase& db, int account_id, bool delete_messages_too);
    static bool cleanLabelledMessages(const QSqlDatabase& db, bool clean_read_only, Label* label);
    static bool cleanImportantMessages(const QSqlDatabase& db, bool clean_read_only, int account_id);
    static bool cleanFeeds(const QSqlDatabase& db, const QStringList& ids, bool clean_read_only, int account_id);
    static bool storeAccountTree(const QSqlDatabase& db, RootItem* tree_root, int account_id);
    static void createOverwriteFeed(const QSqlDatabase& db, Feed* feed, int account_id, int parent_id);
    static void createOverwriteCategory(const QSqlDatabase& db, Category* category, int account_id, int parent_id);
    static bool deleteFeed(const QSqlDatabase& db, int feed_custom_id, int account_id);
    static bool deleteCategory(const QSqlDatabase& db, int id);

    template<typename T>
    static Assignment getCategories(const QSqlDatabase& db, int account_id, bool* ok = nullptr);

    template<typename T>
    static Assignment getFeeds(const QSqlDatabase& db, const QList<MessageFilter*>& global_filters,
                               int account_id, bool* ok = nullptr);

    // Message filters operators.
    static bool purgeLeftoverMessageFilterAssignments(const QSqlDatabase& db, int account_id);
    static MessageFilter* addMessageFilter(const QSqlDatabase& db, const QString& title, const QString& script);
    static void removeMessageFilter(const QSqlDatabase& db, int filter_id, bool* ok = nullptr);
    static void removeMessageFilterAssignments(const QSqlDatabase& db, int filter_id, bool* ok = nullptr);
    static QList<MessageFilter*> getMessageFilters(const QSqlDatabase& db, bool* ok = nullptr);
    static QMultiMap<QString, int> messageFiltersInFeeds(const QSqlDatabase& db, int account_id, bool* ok = nullptr);
    static void assignMessageFilterToFeed(const QSqlDatabase& db, const QString& feed_custom_id, int filter_id,
                                          int account_id, bool* ok = nullptr);
    static void updateMessageFilter(const QSqlDatabase& db, MessageFilter* filter, bool* ok = nullptr);
    static void removeMessageFilterFromFeed(const QSqlDatabase& db, const QString& feed_custom_id, int filter_id,
                                            int account_id, bool* ok = nullptr);

    // Gmail account.
    static QStringList getAllGmailRecipients(const QSqlDatabase& db, int account_id);

  private:
    static QString unnulifyString(const QString& str);

    explicit DatabaseQueries() = default;
};

template<typename T>
QList<ServiceRoot*> DatabaseQueries::getAccounts(const QSqlDatabase& db, const QString& code, bool* ok) {
  QSqlQuery query(db);
  QList<ServiceRoot*> roots;

  if (query.exec(QSL("SELECT * FROM Accounts WHERE type = '%1';").arg(code))) {
    while (query.next()) {
      ServiceRoot* root = new T();

      // Load common data.
      root->setAccountId(query.value(QSL("id")).toInt());

      QNetworkProxy proxy(QNetworkProxy::ProxyType(query.value(QSL("proxy_type")).toInt()),
                          query.value(QSL("proxy_host")).toString(),
                          query.value(QSL("proxy_port")).toInt(),
                          query.value(QSL("proxy_username")).toString(),
                          TextFactory::decrypt(query.value(QSL("proxy_password")).toString()));

      root->setNetworkProxy(proxy);
      root->setCustomDatabaseData(deserializeCustomData(query.value(QSL("custom_data")).toString()));

      roots.append(root);
    }

    if (ok != nullptr) {
      *ok = true;
    }
  }
  else {
    qWarningNN << LOGSEC_DB
               << "Loading of accounts with code"
               << QUOTE_W_SPACE(code)
               << "failed with error:"
               << QUOTE_W_SPACE_DOT(query.lastError().text());

    if (ok != nullptr) {
      *ok = false;
    }
  }

  return roots;
}

template<typename T>
Assignment DatabaseQueries::getCategories(const QSqlDatabase& db, int account_id, bool* ok) {
  Assignment categories;

  // Obtain data for categories from the database.
  QSqlQuery query_categories(db);

  query_categories.setForwardOnly(true);
  query_categories.prepare(QSL("SELECT * FROM Categories WHERE account_id = :account_id;"));
  query_categories.bindValue(QSL(":account_id"), account_id);

  if (!query_categories.exec()) {
    qFatal("Query for obtaining categories failed. Error message: '%s'.", qPrintable(query_categories.lastError().text()));

    if (ok != nullptr) {
      *ok = false;
    }
  }
  else {
    if (ok != nullptr) {
      *ok = true;
    }
  }

  while (query_categories.next()) {
    AssignmentItem pair;

    pair.first = query_categories.value(CAT_DB_PARENT_ID_INDEX).toInt();
    pair.second = new T();

    auto* cat = static_cast<Category*>(pair.second);

    cat->setId(query_categories.value(CAT_DB_ID_INDEX).toInt());
    cat->setCustomId(query_categories.value(CAT_DB_CUSTOM_ID_INDEX).toString());

    if (cat->customId().isEmpty()) {
      cat->setCustomId(QString::number(cat->id()));
    }

    cat->setTitle(query_categories.value(CAT_DB_TITLE_INDEX).toString());
    cat->setDescription(query_categories.value(CAT_DB_DESCRIPTION_INDEX).toString());
    cat->setCreationDate(TextFactory::parseDateTime(query_categories.value(CAT_DB_DCREATED_INDEX).value<qint64>()).toLocalTime());
    cat->setIcon(qApp->icons()->fromByteArray(query_categories.value(CAT_DB_ICON_INDEX).toByteArray()));

    categories << pair;
  }

  return categories;
}

template<typename T>
Assignment DatabaseQueries::getFeeds(const QSqlDatabase& db,
                                     const QList<MessageFilter*>& global_filters,
                                     int account_id,
                                     bool* ok) {
  Assignment feeds;

  // All categories are now loaded.
  QSqlQuery query(db);
  auto filters_in_feeds = messageFiltersInFeeds(db, account_id);

  query.setForwardOnly(true);
  query.prepare(QSL("SELECT * FROM Feeds WHERE account_id = :account_id;"));
  query.bindValue(QSL(":account_id"), account_id);

  if (!query.exec()) {
    qFatal("Query for obtaining feeds failed. Error message: '%s'.", qPrintable(query.lastError().text()));

    if (ok != nullptr) {
      *ok = false;
    }
  }
  else {
    if (ok != nullptr) {
      *ok = true;
    }
  }

  while (query.next()) {
    AssignmentItem pair;

    pair.first = query.value(FDS_DB_CATEGORY_INDEX).toInt();

    Feed* feed = new T();

    // Load common data.
    feed->setTitle(query.value(FDS_DB_TITLE_INDEX).toString());
    feed->setId(query.value(FDS_DB_ID_INDEX).toInt());
    feed->setSource(query.value(FDS_DB_SOURCE_INDEX).toString());
    feed->setCustomId(query.value(FDS_DB_CUSTOM_ID_INDEX).toString());

    if (feed->customId().isEmpty()) {
      feed->setCustomId(QString::number(feed->id()));
    }

    feed->setDescription(QString::fromUtf8(query.value(FDS_DB_DESCRIPTION_INDEX).toByteArray()));
    feed->setCreationDate(TextFactory::parseDateTime(query.value(FDS_DB_DCREATED_INDEX).value<qint64>()).toLocalTime());
    feed->setIcon(qApp->icons()->fromByteArray(query.value(FDS_DB_ICON_INDEX).toByteArray()));
    feed->setAutoUpdateType(static_cast<Feed::AutoUpdateType>(query.value(FDS_DB_UPDATE_TYPE_INDEX).toInt()));
    feed->setAutoUpdateInitialInterval(query.value(FDS_DB_UPDATE_INTERVAL_INDEX).toInt());

    qDebugNN << LOGSEC_CORE
             << "Custom ID of feed when loading from DB is"
             << QUOTE_W_SPACE_DOT(feed->customId());

    // Load custom data.
    feed->setCustomDatabaseData(deserializeCustomData(query.value(QSL("custom_data")).toString()));

    if (filters_in_feeds.contains(feed->customId())) {
      auto all_filters_for_this_feed = filters_in_feeds.values(feed->customId());

      for (MessageFilter* fltr : global_filters) {
        if (all_filters_for_this_feed.contains(fltr->id())) {
          feed->appendMessageFilter(fltr);
        }
      }
    }

    pair.second = feed;

    feeds << pair;
  }

  return feeds;
}

template<typename Categ, typename Fee>
void DatabaseQueries::loadFromDatabase(ServiceRoot* root) {
  QSqlDatabase database = qApp->database()->driver()->connection(root->metaObject()->className());
  Assignment categories = DatabaseQueries::getCategories<Categ>(database, root->accountId());
  Assignment feeds = DatabaseQueries::getFeeds<Fee>(database, qApp->feedReader()->messageFilters(), root->accountId());
  auto labels = DatabaseQueries::getLabelsForAccount(database, root->accountId());

  root->performInitialAssembly(categories, feeds, labels);
}

#endif // DATABASEQUERIES_H
