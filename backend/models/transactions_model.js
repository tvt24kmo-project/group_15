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
    }

}

module.exports = transactions;