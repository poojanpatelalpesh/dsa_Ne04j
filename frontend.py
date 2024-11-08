import tkinter as tk
from tkinter import scrolledtext
import subprocess
import threading
import queue
import os

class InteractiveFrontend:
    def __init__(self):
        self.root = tk.Tk()
        self.root.title("Interactive Query Interface")
        
        # Create GUI elements with extra wide dimensions
        self.input_text = tk.Text(self.root, height=15, width=120)  # Increased width to 120
        self.input_text.pack(padx=10, pady=5)
        
        self.run_button = tk.Button(self.root, text="Send Query", command=self.send_query)
        self.run_button.pack(pady=5)
        
        self.output_text = scrolledtext.ScrolledText(self.root, height=30, width=120)  # Increased width to 120
        self.output_text.pack(padx=10, pady=5)
        
        # Set minimum window size to prevent resizing smaller than the widgets
        self.root.update()
        min_width = self.input_text.winfo_reqwidth() + 20  # Add padding
        min_height = self.input_text.winfo_reqheight() + self.output_text.winfo_reqheight() + 50
        self.root.minsize(min_width, min_height)
        
        # Initialize process-related variables
        self.process = None
        self.output_queue = queue.Queue()
        self.is_running = False
        
        # Start the C++ program when launching the frontend
        self.start_cpp_program()
        
        # Start the output monitoring
        self.start_output_monitor()
        
    def start_cpp_program(self):
        try:
            # Determine the correct executable name
            executable_path = './Database' if os.path.isfile('./Database') else './Database.exe'
            
            if not os.path.isfile(executable_path):
                self.output_text.insert(tk.END, "Error: Executable 'Database' not found.\n")
                return
            
            # Start the C++ program
            self.process = subprocess.Popen(
                [executable_path],
                stdin=subprocess.PIPE,
                stdout=subprocess.PIPE,
                stderr=subprocess.PIPE,
                text=True,
                bufsize=1,  # Line buffered
                universal_newlines=True
            )
            
            self.is_running = True
            
            # Start threads to monitor output
            threading.Thread(target=self.monitor_output, args=(self.process.stdout, "")).start()
            threading.Thread(target=self.monitor_output, args=(self.process.stderr, "Error")).start()
            
            self.output_text.insert(tk.END, "")
        except Exception as e:
            self.output_text.insert(tk.END, f"Failed to start C++ program: {str(e)}\n")
    
    def monitor_output(self, pipe, prefix):
        """Continuously monitor output from the C++ program"""
        try:
            while self.is_running:
                line = pipe.readline()
                if line:
                    self.output_queue.put(f"{prefix}: {line}")
                elif not self.is_running:
                    break
        except Exception as e:
            self.output_queue.put(f"Error in output monitoring: {str(e)}")
    
    def start_output_monitor(self):
        """Check the output queue and update the GUI"""
        try:
            while not self.output_queue.empty():
                message = self.output_queue.get_nowait()
                self.output_text.insert(tk.END, message)
                self.output_text.see(tk.END)  # Auto-scroll to bottom
        except queue.Empty:
            pass
        finally:
            # Schedule the next check
            self.root.after(100, self.start_output_monitor)
    
    def send_query(self):
        """Send input to the C++ program"""
        if not self.process or self.process.poll() is not None:
            self.output_text.insert(tk.END, "The program has ended.\nThe Database has been erased.\n")
            return
        
        query = self.input_text.get("1.0", tk.END).strip()
        if query:
            try:
                # Send the input to the C++ program
                self.process.stdin.write(query + "\n")
                self.process.stdin.flush()
                
                # Clear the input box
                self.input_text.delete("1.0", tk.END)
                
            except Exception as e:
                self.output_text.insert(tk.END, f"Error sending query: {str(e)}\n")
    
    def cleanup(self):
        """Cleanup when closing the window"""
        self.is_running = False
        if self.process:
            try:
                self.process.terminate()
                self.process.wait(timeout=1)
            except:
                self.process.kill()  # Force kill if it doesn't terminate
        self.root.destroy()
    
    def run(self):
        """Start the GUI"""
        self.root.protocol("WM_DELETE_WINDOW", self.cleanup)  # Handle window closing
        self.root.mainloop()

# Create and run the frontend
if __name__ == "__main__":
    frontend = InteractiveFrontend()
    frontend.run()
