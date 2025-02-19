const db = require('../database');

const AC = { // ac = account_cards
    getAllAC: function (callback) {
        return db.query("SELECT * FROM account_cards", callback);
    },

    getByAccountId: function(accountId, callback) {
      return db.query('SELECT * FROM account_cards WHERE account_id = ?', [accountId], callback);
    },

    getByCardId: function(cardId, callback) {
      return db.query('SELECT * FROM account_cards WHERE card_id = ?', [cardId], callback);
    },

    getByBothIds: function(accountId, cardId, callback) {
        return db.query('SELECT * FROM account_cards WHERE account_id = ? AND card_id = ?', [accountId, cardId], callback);
    },

    addAC: function (accountCard, callback) {
        const { account_id, card_id } = accountCard;
        return db.query("INSERT INTO account_cards (account_id, card_id) VALUES (?, ?)",
        [account_id, card_id], callback);
    },

    updateAC: function (accountId, cardId, accountCard, callback) {
        const { new_account_id, new_card_id } = accountCard;
        return db.query("UPDATE account_cards SET account_id=?, card_id=? WHERE account_id=? AND card_id=?",
        [new_account_id, new_card_id, accountId, cardId], callback);
    },

    deleteAC: function (accountId, cardId, callback) {
        return db.query("DELETE FROM account_cards WHERE account_id=? AND card_id=?", [accountId, cardId], callback);
    },

    deleteByAccountId: function(accountId, callback) {
      return db.query('DELETE FROM account_cards WHERE account_id = ?', [accountId], callback);
    },

    deleteByCardId: function(cardId, callback) {
      return db.query('DELETE FROM account_cards WHERE card_id = ?', [cardId], callback);
    },
};

module.exports = AC;