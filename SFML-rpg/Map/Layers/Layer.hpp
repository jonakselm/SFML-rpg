class Layer
{
public:
	Layer();

	void loadTiles(const Json::Value &root, const std::string &group);
	int getTile(const sf::Vector2i &gridPos) const;
	const std::vector<int> getTiles() const;

	const sf::Vector2i &getSize() const;
	const std::string &getGroup() const;

private:
	std::vector<int> m_tiles;
	sf::Vector2i m_size;
	std::string m_group;
};