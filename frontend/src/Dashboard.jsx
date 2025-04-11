import React, { useEffect, useState } from "react";
import "./Dashboard.css";

const Dashboard = () => {
  const [rfidStatus, setRfidStatus] = useState("searching");

  useEffect(() => {
    const timer = setTimeout(() => {
      setRfidStatus("scanned");
    }, 4000); // change status after 4 seconds
    return () => clearTimeout(timer);
  }, []);

  return (
    <div className="dashboard-container">
      <h2 className="dashboard-title">Dashboard Overview</h2>
      <div className="dashboard-cards">
        <div className="dashboard-card">
          <h3 className="dashboard-card-title">Total Books</h3>
          <p className="dashboard-card-value">120</p>
        </div>
        <div className="dashboard-card">
          <h3 className="dashboard-card-title">Registered Users</h3>
          <p className="dashboard-card-value">45</p>
        </div>
      </div>

      <div className="rfid-section">
        <div className={`scanner ${rfidStatus === "searching" ? "pulse" : ""}`}>
          {rfidStatus === "searching" ? (
            <p>🔍 Searching for RFID...</p>
          ) : (
            <p>✅ RFID has been scanned!<br></br>
              <h4>User Details</h4>
              <h5>Alice</h5>
              <h5>2021/E/100</h5>
              <h5>+94751234567</h5>
              {/* <button>Register Book</button> */}
              <button className="nav-button" onClick={(e) => navigate("/register-book")}>
              Register Book
              </button>
            </p> 
          )}
        </div>
      </div>
    </div>
  );
};

export default Dashboard;
