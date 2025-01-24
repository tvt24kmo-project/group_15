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

module.exports = router;
