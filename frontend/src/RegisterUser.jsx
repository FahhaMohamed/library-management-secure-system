import React, { useState } from "react";
import "./RegisterUser.css";

const RegisterUser = () => {
  const [formData, setFormData] = useState({
    username: "",
    email: "",
    password: "",
  });

  const handleChange = (e) => {
    setFormData((prev) => ({
      ...prev,
      [e.target.name]: e.target.value,
    }));
  };

  const handleSubmit = (e) => {
    e.preventDefault();
    console.log("User Registered:", formData);
    // Add your backend call here
  };

  return (
    <div className="register-user-container">
      <h2 className="register-user-title">User Registration</h2>
      <form className="register-user-form" onSubmit={handleSubmit}>
        <input
          type="text"
          name="name"
          placeholder="Full Name"
          value={formData.name}
          onChange={handleChange}
          required
        />
        <input
          type="text"
          name="registration"
          placeholder="Registration Number"
          value={formData.registration}
          onChange={handleChange}
          required
        />
        <input
          type="text"
          name="pnumber"
          placeholder="Phone Number"
          value={formData.pnumber}
          onChange={handleChange}
          required
        />
        <button type="submit">Register User</button>
      </form>
    </div>
  );
};

export default RegisterUser;
