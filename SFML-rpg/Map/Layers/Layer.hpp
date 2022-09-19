class Layer
{
public:
	Layer();

	void loadTiles(const Json::Value &root);
	int getTile(const sf::Vector2i &gridPos) const;
	const std::vector<int> getTiles() const;

	const sf::Vector2i &getSize() const;

private:
	std::vector<int> m_tiles;
	sf::Vector2i m_size;
};