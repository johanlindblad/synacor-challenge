 vault = [
 	%w[* 8 - 1],
 	%w[4 * 11 *],
 	%w[+ 4 - 18],
 	%w[22 - 9 *]
 ]

queue = Queue.new
PathInfo = Struct.new(:weight, :x, :y)

def walk_path(path, vault)
	x = 0
	y = 3
	weight = 22
	operator = nil

	path.each do |direction|
		case direction
		when :north
			y -= 1
		when :south
			y += 1
		when :west	
			x -= 1
		when :east
			x += 1
		else
			raise "Invalid direction #{direction}"
		end

		return PathInfo.new(0, 0, 3) if x == 0 && y == 3

		if operator.nil?
			operator = vault[y][x]
		else
			case operator
			when '+'
				weight += vault[y][x].to_i
			when '-'
				weight -= vault[y][x].to_i
			when '*'
				weight *= vault[y][x].to_i
			else
				raise "Invalid operator #{operator}"
			end
		
			break if x == 3 && y == 0
			operator = nil
		end
	end

	return PathInfo.new(weight, x, y)
end

def valid_directions_from(x, y)
	valid = [:north, :south, :west, :east]

	valid.delete(:north) if y == 0
	valid.delete(:south) if y == 3
	valid.delete(:west) if x == 0
	valid.delete(:east) if x == 3
	valid
end

def is_solution?(info)
	return info.x == 3 && info.y == 0 && info.weight == 30
end

queue.push [:north]
queue.push [:east]

until queue.empty?
	path = queue.pop
	info = walk_path(path, vault)
	if is_solution?(info)
		puts "Solution: #{path.join(', ')}"
		break
	end

	valid_directions_from(info.x, info.y).each do |direction|
		queue.push(path + [direction])
	end
end
