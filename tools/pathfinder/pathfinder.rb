require 'open3'

known_directions = %w{north south west east}.map{|s| s + "\n"}
ignored = %w{ladder}.map{|s| s + "\n"}
initial = File.read('initial').split("\n")

queue = Queue.new

known_directions.map{|d| [d]}.each{|d| queue.push(d)}

while !queue.empty?
	path_end = queue.pop
	total_path = initial + path_end

	output = ''

	Open3.popen3('cd ../../;./main') do |stdin, stdout, stderr, wait_thr|
		stdin.write(total_path.join("\n"))
		stdin.close
		output = stdout.readlines

		output.reverse.each do |line|
			if known_directions.include?(line.gsub("- ", ""))
				known_directions.each do |direction|
					queue.push(path_end + [direction])
				end
			elsif line.include?("There are")
				break
			elsif line.include?("- ")
				next if ignored.include?(line.gsub("- ", ""))
				puts "Possible path: #{path_end.map(&:strip).join(", ")}"
			end
		end
	end
end