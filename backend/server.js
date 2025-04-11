const express = require("express");
const mongoose = require("mongoose");
const cors = require("cors");
const bodyParser = require("body-parser");

const app = express();
const PORT = 5000;

app.use(cors());
app.use(bodyParser.json());

mongoose.connect("mongodb+srv://shathuthepath:xwQPU7yHUIy38iaY@cluster0.hqobcis.mongodb.net/?retryWrites=true&w=majority&appName=Cluster0", {
  useNewUrlParser: true,
  useUnifiedTopology: true,
});

const TempTagSchema = new mongoose.Schema({
  tagId: String,
  createdAt: { type: Date, default: Date.now, expires: 60 }, // expires in 60 sec
});

const UserSchema = new mongoose.Schema({
  name: String,
  registration: String,
  pnumber: String,
  tagId: String,
});

const TempTag = mongoose.model("TempTag", TempTagSchema);
const User = mongoose.model("User", UserSchema);

app.post("/api/tag", async (req, res) => {
  const { tagId } = req.body;
  if (!tagId) return res.status(400).json({ error: "Tag ID missing" });

  const tag = new TempTag({ tagId });
  await tag.save();

  res.json({ message: "Tag received" });
});


app.get("/api/latest-tag", async (req, res) => {
  const latestTag = await TempTag.findOne().sort({ createdAt: -1 });
  if (!latestTag) return res.status(404).json({ error: "No tag found" });

  res.json({ tagId: latestTag.tagId });
});


app.post("/api/register", async (req, res) => {
  const { name, registration, pnumber, tagId } = req.body;
  if (!name || !registration || !pnumber || !tagId) {
    return res.status(400).json({ error: "Missing fields" });
  }

  const user = new User({ name, registration, pnumber, tagId });
  await user.save();

  res.json({ message: "User registered successfully" });
});

app.listen(PORT, () => {
  console.log(`Server running on http://localhost:${PORT}`);
});
