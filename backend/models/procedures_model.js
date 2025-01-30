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
        db.query(
            "CALL getAccountType(?, @account_type); SELECT @account_type AS account_type;",
            [idaccount],
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

    GetCustomerTransactions: function (idaccount, page, callback) {
        db.query(
            "CALL GetCustomerTransactions(?, ?);",
            [idaccount, page],
            function (err, results) {
                if (err) {
                    return callback(err, null);
                }
                callback(null, results[0]); // Return the first result set
            }
        );
    },
};

module.exports = procedures;
