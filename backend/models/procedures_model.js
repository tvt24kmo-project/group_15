const db = require('../database');

const procedures = {
    withdraw: function (account_id, amount, callback) {
        return db.query("CALL withdraw(?, ?)", [account_id, amount], callback);
    }
};

module.exports = procedures;