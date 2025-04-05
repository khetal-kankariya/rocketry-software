# Start the Python backend server
Write-Host "Starting Python backend server..."
Set-Location -Path "backend"

# Start the backend server in a new PowerShell process
if (Test-Path ".\.venv\Scripts\Activate.ps1") {
    Write-Host "Activating Python virtual environment..."
    .\.venv\Scripts\Activate.ps1
} else {
    Write-Host "No virtual environment found in 'backend/.venv'"
}

Start-Process "python" -ArgumentList "server.py"  # Replace with your Python command (e.g., `python app.py`)

# Go back to the root directory
Set-Location -Path ".."

# Start the Svelte frontend server
Write-Host "Starting Svelte frontend..."
Set-Location -Path "frontend"

# Run the frontend server (assuming you're using npm/yarn to run Svelte)
Start-Process "npm" -ArgumentList "run dev"  # Or "yarn dev" if using yarn

# Go back to the root directory
Set-Location -Path ".."

# Optionally, you can open the frontend in the default browser automatically
Start-Process "http://localhost:3000"  # Adjust if you use a different port
