import React from "react";
import { BrowserRouter as Router, Routes, Route } from "react-router-dom";
import Navbar from "./Navbar";
import LoginPage from "./LoginPage";
import Dashboard from "./Dashboard";
import Transactions from "./Transactions";
import RegisterUser from "./RegisterUser";
import RegisterBook from "./RegisterBook";
import "./App.css";

function App() {
  return (
    <Router>
      <div className="app-container">
        <Navbar />
        <Routes>
          <Route path="/" element={<LoginPage />} />
          <Route path="/dashboard" element={<Dashboard />} />
          <Route path="/transactions" element={<Transactions />} />
          <Route path="/register-user" element={<RegisterUser />} />
          <Route path="/register-book" element={<RegisterBook />} />

        </Routes>
      </div>
    </Router>
  );
}

export default App;
