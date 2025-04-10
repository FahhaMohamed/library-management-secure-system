import React from "react";
import "./Dashboard.css";

const Dashboard = () => {
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
        {/* <div className="dashboard-card">
          <h3 className="dashboard-card-title">Unauthorized Attempts</h3>
          <p className="dashboard-card-value">3</p>
        </div> */}
      </div>
    </div>
  );
};

export default Dashboard;
