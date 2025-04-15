# The Flight Software and GCS UI for InSpace Rocketry Competition 2025.

## Flight Software

go into this dir and upload code to the rp2040zero

```zsh
cd sfw
```
```zsh
cd rocket-sfw #for rocket flight software
```
```zsh
cd payload-sfw #for payload flight software
```

### Installing

## UI

### Frontend

#### Installing javascript dependencies

Step 1. Install Nodejs from https://nodejs.org/en

Step 2. 
```zsh
cd gcs/frontened

npm install
```

### Backend

```zsh
cd gcs/backend
```

### Setup virtual environment

```zsh
python -m venv .venv
```

### Activating virtual environment

For Windows:

```zsh
.venv\Scripts\Activate
```

For Linux/WSL:

```zsh
source venv/bin/activate
```

#### Installing python dependencies

```zsh
pip install -r requirements.txt
```
