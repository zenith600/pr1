import random
import tkinter as tk


class Maze:
    def __init__(self, rows, cols):
        self.rows = rows
        self.cols = cols
        self.grid = [[Cell(row, col) for col in range(cols)] for row in range(rows)]
        self.current_cell = self.grid[0][0]
        self.stack = []
        self.solved = False
        self.path = []

    def generate_maze(self):
        while True:
            self.current_cell.visited = True
            neighbors = self.get_unvisited_neighbors(self.current_cell.row, self.current_cell.col)
            if neighbors:
                next_cell = random.choice(neighbors)
                self.remove_wall(self.current_cell, next_cell)
                self.stack.append(self.current_cell)
                self.current_cell = next_cell
            elif self.stack:
                self.current_cell = self.stack.pop()
            else:
                break

    def get_unvisited_neighbors(self, row, col):
        neighbors = []
        if row > 0 and not self.grid[row - 1][col].visited:
            neighbors.append(self.grid[row - 1][col])
        if row < self.rows - 1 and not self.grid[row + 1][col].visited:
            neighbors.append(self.grid[row + 1][col])
        if col > 0 and not self.grid[row][col - 1].visited:
            neighbors.append(self.grid[row][col - 1])
        if col < self.cols - 1 and not self.grid[row][col + 1].visited:
            neighbors.append(self.grid[row][col + 1])
        return neighbors

    def remove_wall(self, current_cell, next_cell):
        if current_cell.row == next_cell.row:
            if current_cell.col < next_cell.col:
                current_cell.walls['right'] = False
                next_cell.walls['left'] = False
            else:
                current_cell.walls['left'] = False
                next_cell.walls['right'] = False
        elif current_cell.col == next_cell.col:
            if current_cell.row < next_cell.row:
                current_cell.walls['down'] = False
                next_cell.walls['up'] = False
            else:
                current_cell.walls['up'] = False
                next_cell.walls['down'] = False

    def solve_maze(self):
        self._reset_visited()
        self._solve_recursive(self.grid[0][0])

    def _solve_recursive(self, cell):
        cell.visited = True
        self.path.append(cell)
        if cell == self.grid[self.rows - 1][self.cols - 1]:
            self.solved = True
            return
        neighbors = self.get_valid_neighbors(cell.row, cell.col)
        for neighbor in neighbors:
            if not neighbor.visited:
                self._solve_recursive(neighbor)
                if self.solved:
                    return
        self.path.remove(cell)

    def get_valid_neighbors(self, row, col):
        neighbors = []
        if not self.grid[row][col].walls['up'] and row > 0:
            neighbors.append(self.grid[row - 1][col])
        if not self.grid[row][col].walls['down'] and row < self.rows - 1:
            neighbors.append(self.grid[row + 1][col])
        if not self.grid[row][col].walls['left'] and col > 0:
            neighbors.append(self.grid[row][col - 1])
        if not self.grid[row][col].walls['right'] and col < self.cols - 1:
            neighbors.append(self.grid[row][col + 1])

        return neighbors

    def _reset_visited(self):
        for row in self.grid:
            for cell in row:
                cell.visited = False

    def draw(self, canvas):
        canvas.delete(tk.ALL)
        cell_width = 30
        cell_height = 30

        for row in self.grid:
            for cell in row:
                x = cell.col * cell_width
                y = cell.row * cell_height
                if cell.walls['up']:
                    canvas.create_line(x, y, x + cell_width, y, width=2)
                if cell.walls['down']:
                    canvas.create_line(x, y + cell_height, x + cell_width, y + cell_height, width=2)
                if cell.walls['left']:
                    canvas.create_line(x, y, x, y + cell_height, width=2)
                if cell.walls['right']:
                    canvas.create_line(x + cell_width, y, x + cell_width, y + cell_height, width=2)

                if cell == self.current_cell:
                    canvas.create_rectangle(x, y, x + cell_width, y + cell_height, fill='green')
                elif cell == self.grid[self.rows - 1][self.cols - 1]:
                    canvas.create_rectangle(x, y, x + cell_width, y + cell_height, fill='red')
                elif cell in self.path:
                    canvas.create_rectangle(x, y, x + cell_width, y + cell_height, fill='light blue')

        if self.solved:
            canvas.create_text((self.cols - 1) * cell_width + cell_width // 2,
                               (self.rows - 1) * cell_height + cell_height // 2,
                               text="Solved!", font=("Arial", 16, "bold"), fill="blue")


class Cell:
    def __init__(self, row, col):
        self.row = row
        self.col = col
        self.visited = False
        self.walls = {'up': True, 'down': True, 'left': True, 'right': True}


maze = None


def generate_maze():
    global maze
    rows = int(rows_entry.get())
    cols = int(cols_entry.get())
    maze = Maze(rows, cols)
    maze.generate_maze()
    maze_canvas.delete(tk.ALL)
    maze.draw(maze_canvas)


def solve_maze():
    global maze
    maze.solve_maze()
    maze.draw(maze_canvas)


# Create the main window
window = tk.Tk()
window.title("Maze Generator and Solver")

# Create the maze canvas
maze_canvas = tk.Canvas(window, width=600, height=400)
maze_canvas.pack()

# Create the input fields and buttons
input_frame = tk.Frame(window)
input_frame.pack(pady=10)

rows_label = tk.Label(input_frame, text="Rows:")
rows_label.grid(row=0, column=0, padx=5)

rows_entry = tk.Entry(input_frame)
rows_entry.grid(row=0, column=1, padx=5)

cols_label = tk.Label(input_frame, text="Columns:")
cols_label.grid(row=0, column=2, padx=5)

cols_entry = tk.Entry(input_frame)
cols_entry.grid(row=0, column=3, padx=5)

generate_button = tk.Button(window, text="Generate Maze", command=generate_maze)
generate_button.pack(pady=5)

solve_button = tk.Button(window, text="Solve Maze", command=solve_maze)
solve_button.pack(pady=5)

# Run the main event loop
window.mainloop()
