import React, { useState } from "react";
import "./RegisterBook.css";

const RegisterBook = () => {
  const [bookData, setBookData] = useState({
    title: "",
    author: "",
    isbn: "",
  });

  const handleChange = (e) => {
    setBookData((prev) => ({
      ...prev,
      [e.target.name]: e.target.value,
    }));
  };

  const handleSubmit = (e) => {
    e.preventDefault();
    console.log("Book Registered:", bookData);
    // Hook this up to backend as needed
  };

  return (
    <div className="register-book-container">
      <h2 className="register-book-title">Book Registration</h2>
      <form className="register-book-form" onSubmit={handleSubmit}>
        <input
          type="text"
          name="title"
          placeholder="Book Title"
          value={bookData.title}
          onChange={handleChange}
          required
        />
        <input
          type="text"
          name="author"
          placeholder="Author"
          value={bookData.author}
          onChange={handleChange}
          required
        />
        <input
          type="text"
          name="isbn"
          placeholder="ISBN"
          value={bookData.isbn}
          onChange={handleChange}
          required
        />
        <button type="submit">Register Book</button>
      </form>
    </div>
  );
};

export default RegisterBook;
