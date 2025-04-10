import React from "react";
import { Link } from "react-router-dom";
import "./Navbar.css";

const Navbar = () => {
  return (
    <nav className="navbar">
      <div className="navbar-container">
        <div className="navbar-links">
          <Link to="/dashboard" className="navbar-link">
            <h1>Smart Library</h1>
          </Link>
        </div>

        {/* <h1 className="navbar-brand">Smart Library</h1> */}

        <div className="navbar-links">
          {/* <Link to="/dashboard" className="navbar-link">
            Dashboard
          </Link> */}
          <Link to="/transactions" className="navbar-link">
            Transactions
          </Link>
          <Link to="/register-book" className="navbar-link">
            Register Book
          </Link>
          <Link to="/register-user" className="navbar-link">
            Register User
          </Link>
        </div>
      </div>
    </nav>
  );
};

export default Navbar;
