const db = require('../database');

const procedures = {
    withdraw: function (account_id, amount, callback) {
        return db.query("CALL WithdrawFunds(?, ?)", [account_id, amount], callback);
    },

    getAccountType: function (account_id, callback) {
        return db.query("CALL GetAccountType (?)", [account_id], callback);
    },
};

module.exports = procedures;