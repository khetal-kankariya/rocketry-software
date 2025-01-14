# Prerequisites:

The Flight Software and GCS UI for InSpace Rocketry Competition 2025.

## Flight Software

go into this dir and upload code to the rp2040zero

```zsh
cd rocketry_flight_sfw
```

### Installing

## UI

### Frontend

```zsh
cd rocketry-ui/frontend
```

#### Installing javascript dependencies

bun is preferred

```zsh
#bun
bun install

#pnpm
pnpm install

#npm
npm install
```

##### Installing bun

```zsh
powershell -c "irm bun.sh/install.ps1|iex"
```

### Backend

```zsh
cd rocketry-ui/backend
```

#### Installing python dependencies

```zsh
pip install -r requirements.txt
```
