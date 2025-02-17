const express = require('express');
const router = express.Router();
const procedures = require('../models/procedures_model');

router.post('/withdraw', function (req, res) {
    const { account_id, amount } = req.body;

    // Validate input
    if (!account_id || !amount || amount <= 0) {
        return res.status(400).json({ error: 'Invalid input: account_id and a positive amount are required.' });
    }

    // Call the withdraw procedure
    procedures.withdraw(account_id, amount, function (err, result) {
        if (err) {
            console.error(err); // Log error for debugging
            return res.status(500).json({ error: 'An error occurred while processing the withdrawal.' });
        }

        // Extract status from result
        const status = result.status;

        // Return response based on status
        if (status === 'Success') {
            res.json({ message: 'Withdrawal successful.' });
        } else if (status === 'Insufficient funds') {
            res.status(400).json({ error: 'Insufficient funds.' });
        } else {
            res.status(500).json({ error: 'Unexpected status: ' + status });
        }
    });
});


router.post('/transfer', function (req, res) {
    console.log("Request received:", req.body); // Tulostetaan mitä postman lähettää

    const { sender_account, receiver_account, transfer_amount } = req.body;

    // Validate input
    if (!sender_account || !receiver_account || !transfer_amount || transfer_amount <= 0) {
        return res.status(400).json({ error: 'Invalid input: sender_account, receiver_account, and a positive transfer_amount are required.' });
    }

    // Call the transfer procedure
    procedures.transfer(sender_account, receiver_account, transfer_amount, function (err, result) {
        if (err) {
            console.error(err); // Log error for debugging
            if (err.sqlState) { // jos virhe tulee sql puolelta
                return res.status(400).json({ error: err.message }); //heitetään sql virhe ulos 
            }
            return res.status(500).json({ error: 'An error occurred while processing the transfer.' });
        }
        res.json({ message: 'Transfer successful.' }); // jos ei tullut virheitä niin palautetaan onnistunut viesti
    });
});


router.post('/getAccountType', function (req, res) {
    const { account_id } = req.body;

    // Validate input
    if (!account_id) {
        return res.status(400).json({ error: 'Invalid input: account_id is required.' });
    }

    procedures.getAccountType(account_id, function (err, result) {
        if (err) {
            console.error(err); // Log error for debugging
            res.status(500).json({ error: 'An error occurred while fetching account type.' });
        } else {
            res.json(result);
        }
    });
});

router.get('/GetCustomerTransactions', function (req, res) {
    const { idaccount, page } = req.query;

    // Validate input
    if (!idaccount || !page || page < 1) {
        return res.status(400).json({ 
            error: 'Invalid input: both idaccount and page (positive integer) are required.' 
        });
    }

    procedures.GetCustomerTransactions(idaccount, page, function (err, result) {
        if (err) {
            console.error(err); // Log error for debugging
            res.status(500).json({ 
                error: 'An error occurred while fetching transactions.' 
            });
        } else {
            res.json(result);
        }
    });
});

module.exports = router;