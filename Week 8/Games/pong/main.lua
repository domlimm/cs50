WINDOW_WIDTH = 1280
WINDOW_HEIGHT = 720

VIRTUAL_WIDTH = 432
VIRTUAL_HEIGHT = 243

PADDLE_SPEED = 200 -- 200 pixels / second

-- from: https://github.com/Ulydev/push
Class = require 'class'
push = require 'push' -- importing file

require 'Ball'
require 'Paddle'

function love.load()
  math.randomseed(os.time())

  love.graphics.setDefaultFilter('nearest', 'nearest')

  smallFont = love.graphics.newFont('font.ttf', 8)
  scoreFont = love.graphics.newFont('font.ttf', 32)
  victoryFont = love.graphics.newFont('font.ttf', 24)

  sounds = {
    ['paddle_hit'] = love.audio.newSource('paddle_hit.wav', 'static'),
    ['point_scored'] = love.audio.newSource('point_scored.wav', 'static'),
    ['wall_hit'] = love.audio.newSource('wall_hit.wav', 'static')
  }

  -- init window with virtual resolution
  push:setupScreen(VIRTUAL_WIDTH, VIRTUAL_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT, {
    fullscreen = false,
    vsync = true,
    resizable = true
  })

  love.window.setTitle('Pong')

  player1Score = 0
  player2Score = 0

  servingPlayer = math.random(2) == 1 and 1 or 2

  winningPlayer = 0

  paddle1 = Paddle(5, 20, 5, 20)
  paddle2 = Paddle(VIRTUAL_WIDTH - 10, VIRTUAL_HEIGHT - 30, 5, 20)
  ball = Ball(VIRTUAL_WIDTH / 2 - 2, VIRTUAL_HEIGHT / 2 - 2, 5, 5)

  if servingPlayer == 1 then
    ball.dx = 100
  else
    ball.dx = -100
  end

  ballX = VIRTUAL_WIDTH / 2 - 2
  ballY = VIRTUAL_HEIGHT / 2 - 2

  -- D - Delta
  ballDX = math.random(2) == 1 and -100 or 100
  ballDY = math.random(-50, 50)

  gameState = 'start'
end

function love.resize(w, h)
  push:resize(w, h)
end

-- dt - delta time
function love.update(dt)
  if gameState == 'play' then
    if ball.x < 0 then
      servingPlayer = 1
      player2Score = player2Score + 1
      ball:reset()

      sounds['point_scored']:play()

      if player2Score >= 2 then
        gameState = 'victory'
        winningPlayer = 2
      else
        gameState = 'serve'
      end
    end

    if ball.x > VIRTUAL_WIDTH then
      player1Score = player1Score + 1
      servingPlayer = 2
      ball:reset()

      sounds['point_scored']:play()
      
      if player1Score >= 2 then
        gameState = 'victory'
        winningPlayer = 1
      else
        gameState = 'serve'
      end
    end

    if ball:collides(paddle1) then
      -- deflect ball to the right
      ball.dx = -ball.dx * 1.03
      ball.x = paddle1.x + 5
      sounds['paddle_hit']:play()
    end

    if ball:collides(paddle2) then
      -- deflect ball to the left
      ball.dx = -ball.dx * 1.03
      ball.x = paddle2.x - 4
      sounds['paddle_hit']:play()
    end

    if ball.y <= 0 then
      -- deflect ball down
      ball.dy = -ball.dy
      ball.y = 0

      sounds['wall_hit']:play()
    end

    if ball.y >= VIRTUAL_HEIGHT - 4 then
      -- deflect ball down
      ball.dy = -ball.dy
      ball.y = VIRTUAL_HEIGHT - 4

      sounds['wall_hit']:play()
    end
  end

  -- player 1
  if love.keyboard.isDown('w') then
    paddle1.dy = -PADDLE_SPEED
    -- player1Y = math.max(0, player1Y + -PADDLE_SPEED * dt)
  elseif love.keyboard.isDown('s') then
    paddle1.dy = PADDLE_SPEED
    -- player1Y = math.min(VIRTUAL_HEIGHT - 20, player1Y + PADDLE_SPEED * dt)
  else
    paddle1.dy = 0
  end

  -- AI
  ai(ball)

  -- -- player 2
  -- if love.keyboard.isDown('up') then
  --   paddle2.dy = -PADDLE_SPEED
  --   -- player2Y = math.max(0, player2Y + -PADDLE_SPEED * dt)
  -- elseif love.keyboard.isDown('down') then
  --   paddle2.dy = PADDLE_SPEED
  --   -- player2Y = math.min(VIRTUAL_HEIGHT - 20, player2Y + PADDLE_SPEED * dt)
  -- else
  --   paddle2.dy = 0
  -- end

  if gameState == 'play' then
    ball:update(dt)
  end

  paddle1:update(dt)
  paddle2:update(dt)
end

-- function reset_ball()
--   -- Reset ball position to default
--   ballX = VIRTUAL_WIDTH / 2 - 2
--   ballY = VIRTUAL_HEIGHT / 2 - 2

--   -- D - Delta
--   ballDX = math.random(2) == 1 and -100 or 100
--   ballDY = math.random(-50, 50) * 1.5
-- end

function love.keypressed(key) -- Callback fn; used alone
  if key == 'escape' then
    love.event.quit()
  elseif key == 'enter' or key == 'return' then -- mac uses return for enter
    if gameState == 'start' then
      gameState = 'serve'
    elseif gameState == 'victory' then
      gameState = 'start'
      player1Score = 0
      player2Score = 0
    elseif gameState == 'serve' then
      gameState = 'play'
    end
    -- elseif gameState == 'play' then
    --   gameState = 'start'
    --   ball:reset()
      -- reset_ball() - Ball.lua class takes care of reset
    -- end
  end
end

function love.draw()
  -- - x axis > left = subtract, right = add
  -- | y axis > up = subtract, down = add

  push:apply('start')

  love.graphics.clear(40 / 255, 45 / 255, 52 / 255, 255 / 255)

  -- Paddles
  -- love.graphics.rectangle('fill', VIRTUAL_WIDTH - 10, player2Y, 5, 20)

  love.graphics.setFont(smallFont)

  displayScore()

  if gameState == 'start' then
    love.graphics.setFont(smallFont)
    love.graphics.printf("Welcome to Pong!", 0, 10, VIRTUAL_WIDTH, 'center')
    love.graphics.printf("Press Enter to Play!", 0, 20, VIRTUAL_WIDTH, 'center')
  elseif gameState == 'serve' then
    love.graphics.setFont(smallFont)
    love.graphics.printf("Player " .. tostring(servingPlayer) .. "'s turn!", 0, 10, VIRTUAL_WIDTH, 'center')
    love.graphics.printf("Press Enter to Serve!", 0, 20, VIRTUAL_WIDTH, 'center')
  elseif gameState == 'victory' then
    love.graphics.setFont(victoryFont)
    love.graphics.printf("Player " .. tostring(winningPlayer) .. " wins!", 0, 10, VIRTUAL_WIDTH, 'center')
    love.graphics.setFont(smallFont)
    love.graphics.printf("Press Enter to Serve!", 0, 42, VIRTUAL_WIDTH, 'center')
  elseif gameState == 'play' then

  end

  -- if gameState == 'start' then
  --   love.graphics.printf('Hello Start State!', 0, 20, VIRTUAL_WIDTH, 'center')
  -- else
  --   love.graphics.printf('Hello Play State!', 0, 20, VIRTUAL_WIDTH, 'center')
  -- end

  paddle1:render()
  paddle2:render()

  ball:render()

  displayFPS()

  push:apply('end')
end

function ai(ball)
  if paddle2:moveDown(ball) then
      paddle2.dy = -PADDLE_SPEED
  elseif paddle2:moveUp(ball) then
      paddle2.dy = PADDLE_SPEED
  else
      paddle2.dy = 0
  end
end

function displayScore()
  love.graphics.setFont(scoreFont)
  love.graphics.print(tostring(player1Score), VIRTUAL_WIDTH / 2 - 50, VIRTUAL_HEIGHT / 3)
  love.graphics.print(tostring(player2Score), VIRTUAL_WIDTH / 2 + 30, VIRTUAL_HEIGHT / 3)
end

function displayFPS()
  love.graphics.setColor(0, 1, 0, 1)
  love.graphics.setFont(smallFont)
  love.graphics.print('FPS: ' .. tostring(love.timer.getFPS()), 40, 20)
  love.graphics.setColor(1, 1, 1, 1)
end