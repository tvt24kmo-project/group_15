const db = require('../database');

const procedures = {
    withdraw: function (idaccount, amount, callback) {
        // Multi-statement query: CALL procedure + SELECT @status
        db.query(
            "CALL WithdrawFunds(?, ?, @status); SELECT @status AS status;",
            [idaccount, amount],
            function (err, results) {
                if (err) {
                    return callback(err, null);
                }
                // `results` is an array of result sets. The second result set contains the @status value.
                const statusResult = results[1]; // Second result set: SELECT @status AS status
                callback(null, statusResult[0]); // Return the first row of the second result set
            }
        );
    },

    getAccountType: function (idaccount, callback) {
        return db.query("CALL GetAccountType(?)", [idaccount], callback);
    },
};

module.exports = procedures;
