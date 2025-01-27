#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"
#include "judger_interfaces/msg/answer_num.hpp"
#include "judger_interfaces/msg/my_answer.hpp"
#include "judger_interfaces/msg/overall_info.hpp"
#include "judger_interfaces/msg/road_info.hpp"
#include "judger_interfaces/msg/src_des_info.hpp"
#include "judger_interfaces/srv/my_service.hpp"
#include <vector>
#include <queue>
#include <unordered_map>
#include <memory>

using namespace std::placeholders;

struct RoadInfo {
    int source;

    int destination;

    int length;
};

class TransportationHub : public rclcpp::Node {
public:
    TransportationHub() : Node("transportation_hub") {
        subscription_RoadInfo = this->create_subscription<judger_interfaces::msg::RoadInfo>(
            "road_info", 10, std::bind(&TransportationHub::InputDataCallback, this, std::placeholders::_1));
        // question话题
        subscription_ = this->create_subscription<judger_interfaces::msg::OverallInfo>(
            "question", 10, std::bind(&TransportationHub::topic_callback, this, _1));
        // judger_server服务
        client_ = this->create_client<judger_interfaces::srv::MyService>("judger_server");

        // 初始化
        for (auto &road: road_info_) {
            adj_list_[road.source].push_back({road.destination, road.length});
            adj_list_[road.destination].push_back({road.source, road.length});
        }
    }

private:
    void InputDataCallback(const judger_interfaces::msg::RoadInfo::SharedPtr msg) {
    }
    void topic_callback(const judger_interfaces::msg::OverallInfo::SharedPtr msg) {

        int number_of_cities = msg->number_of_cities;

        int number_of_roads = msg->number_of_roads;

        road_info_.clear();
        // 遍历每条道路信息
        for (int i = 0; i < number_of_roads; ++i) {
//            RoadInfo road = {msg->infos[2 + i * 3], msg->infos[3 + i * 3], msg->infos[1 + i * 3]};
            InputDataCallback(const judger_interfaces::msg::RoadInfo::SharedPtr msg);
            road_info_.push_back(road);
        }
        // 起点城市
        int src_city = msg->src_city;
        // 终点城市
        int des_city = msg->des_city;

        // 查找
        auto path = dijkstra(src_city, des_city);

        // 构造请求并发送给judger_server

        if (!path.empty()) {
            send_vector_to_serial(path);//将path传给judger
        } else {
            RCLCPP_INFO(this->get_logger(), "No path found ");
        }
    }
    }

    std::vector<int> dijkstra(int start, int end) {
        // 优先队列来存储，距离从小到大
        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int> >, std::greater<> > pq;
        // 到起点的距离
        std::unordered_map<int, int> dist;
        // 前驱节点，构建最短路径
        std::unordered_map<int, int> prev;
        // 初始化
        for (const auto &entry: adj_list_) {
            dist[entry.first] = 100000000;
        }
        dist[start] = 0;
        // 加入优先队列
        pq.push({0, start});

        // 当优先队列不为空时，循环处理
        while (!pq.empty()) {
            // 距离起点最近
            int u = pq.top().second;
            pq.pop();

            // 如果当前节点是终点，退出循环
            if (u == end) break;

            // 遍历当前节点的所有邻居节点
            for (const auto &[v, weight]: adj_list_[u]) {
                // 如果通过当前节点距离更短
                if (dist[u] + weight < dist[v]) {
                    // 更新到起点的距离
                    dist[v] = dist[u] + weight;
                    // 记录前驱节点
                    prev[v] = u;
                    // 加入优先队列
                    pq.push({dist[v], v});
                }
            }
        }

        if (dist[end] == 100000000) return {};
        std::vector<int> path;
        // 构建最短路径
        for (int at = end; at != start; at = prev[at]) {
            path.push_back(at);
        }
        // 将起点加入路径
        path.push_back(start);
        // 反转路径，使其从起点到终点
        std::reverse(path.begin(), path.end());
        return path;
    }

    rclcpp::Subscription<judger_interfaces::msg::OverallInfo>::SharedPtr subscription_;// 订阅者，用于订阅question话题

    rclcpp::Subscription<judger_interfaces::msg::RoadInfo>::SharedPtr subscription_RoadInfo;// 订阅者，用于订阅question话题

    rclcpp::Subscription<judger_interfaces::msg::MyAnswer>::SharedPtr subscription_MyAnswer;// 订阅者，用于订阅question话题

    rclcpp::Client<judger_interfaces::srv::MyService>::SharedPtr client_;// 客户端，用于连接到judger_server服务

    std::unordered_map<int, std::vector<std::pair<int, int> > > adj_list_;// 邻接表，用于存储图结构

    std::vector<RoadInfo> road_info_; // 存储道路信息
};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<TransportationHub>());

    rclcpp::shutdown();

    return 0;
}
