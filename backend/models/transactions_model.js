const db = require('../database');

const transactions = {
    // Hae kaikki tapahtumat
    getAllTransactions: function (callback) {
        return db.query("SELECT * FROM transactions", callback);
    },

    // Lisää uusi tapahtuma tietokantaan
    addTransaction: function (transactions_data, callback) {
        return db.query("INSERT INTO transactions(account_id, amount, transaction_type) values(?,?,?)",
            [transactions_data.account_id, transactions_data.amount, transactions_data.transaction_type], callback);
    },

    // Poista tapahtuma id:n perusteella
    removeTransaction: function (id, callback) {
        return db.query("DELETE FROM transactions WHERE idtransactions=?", [id], callback);
    },

    // Päivitä tapahtuma id:n perusteella
    updateTransaction: function (id, update_transactions_data, callback) {
        return db.query("UPDATE transactions SET account_id=?, amount=?, transaction_type=? WHERE idtransactions=?",
            [update_transactions_data.account_id, update_transactions_data.amount, update_transactions_data.transaction_type, id], callback);
    },
    getIbanHistory: function (account_id, callback) {
        return db.query("SELECT DATE_FORMAT(transaction_date, '%d.%m.%Y %H:%i:%s') AS 'aika', account_id AS 'tililtä', amount AS 'määrä', accounts.balance AS 'saldo', \
            transaction_type AS 'tapahtuma' from transactions JOIN accounts ON transactions.account_id = accounts.idaccount \
            WHERE accounts.accountiban = ?;", [account_id], callback);
    }

}

module.exports = transactions;