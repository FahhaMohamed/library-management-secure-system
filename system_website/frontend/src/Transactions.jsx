import React from "react";
import "./Transactions.css";

const Transactions = () => {
  const sampleData = [
    { id: 1, user: "Alice", book: "C++ Basics", type: "Borrow", time: "10:30 AM" },
    { id: 2, user: "Bob", book: "Python 101", type: "Return", time: "11:00 AM" },
    { id: 3, user: "Charlie", book: "IoT Design", type: "Borrow", time: "12:15 PM" },
  ];

  return (
    <div className="transactions-container">
      <h2 className="transactions-title">Transaction Logs</h2>
      <table className="transactions-table">
        <thead>
          <tr>
            <th>No</th>
            <th>Member</th>
            <th>Book</th>
            <th>Type</th>
            <th>Time</th>
          </tr>
        </thead>
        <tbody>
          {sampleData.map((entry) => (
            <tr key={entry.id}>
              <td>{entry.id}</td>
              <td>{entry.user}</td>
              <td>{entry.book}</td>
              <td>{entry.type}</td>
              <td>{entry.time}</td>
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
};

export default Transactions;
