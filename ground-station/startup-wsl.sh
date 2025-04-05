#!/bin/zsh

# Start the Python backend server
echo "Starting Python backend server..."
cd backend
# Assuming the backend is running with a Python framework like Flask or Django
# Replace `python app.py` with your specific command to start the Python server
source .venv/bin/activate
nohup python server.py &  # or `python3 app.py` or whatever you use to start your server

# Go back to the root directory
cd ..

# Start the Svelte frontend
echo "Starting Svelte frontend..."
cd frontend
# Run the frontend server (assuming you're using the standard npm/yarn command)
npm run dev &  # or `yarn dev`

# Go back to the root directory
cd ..

# Optionally, you can open a browser tab with the frontend running
# open http://localhost:3000  # Uncomment if you want to auto open a browser
