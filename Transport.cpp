#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"
#include "judger_interfaces/msg/my_answer.hpp"
#include "judger_interfaces/msg/overall_info.hpp"
#include "judger_interfaces/msg/road_info.hpp"
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
        // question话题
        subscription_ = this->create_subscription<judger_interfaces::msg::OverallInfo>(
            "question",
            10,
            std::bind(&TransportationHub::topic_callback, this,std::placeholders:: _1)
            );
        // judger_server服务
        client_ = this->create_client<judger_interfaces::srv::MyService>("judger_server");

    }

private:
    rclcpp::Subscription<judger_interfaces::msg::OverallInfo>::SharedPtr subscription_;// 订阅者，用于订阅question话题

    rclcpp::Subscription<judger_interfaces::msg::RoadInfo>::SharedPtr subscription_RoadInfo;// 订阅者，用于订阅question话题

    rclcpp::Subscription<judger_interfaces::msg::MyAnswer>::SharedPtr subscription_MyAnswer;// 订阅者，用于订阅question话题

    rclcpp::Client<judger_interfaces::srv::MyService>::SharedPtr client_;// 客户端，用于连接到judger_server服务

    std::unordered_map<int, std::vector<std::pair<int, int> > > adj_list_;// 邻接表，用于存储图结构

    rclcpp::TimerBase::SharedPtr timer;//用于发送信息

    std::vector<RoadInfo> road_info_; // 存储道路信息

    std::vector<int> dijkstra(int start, int end) {
        std::priority_queue<std::pair<int, int>, std::vector<std::pair<int, int> >, std::greater<> > pq;// 优先队列,距离从小到大

        std::unordered_map<int, int> dist;// 到起点的距离

        std::unordered_map<int, int> prev;// 前驱节点构建最短路径

        for (const auto &entry: adj_list_)// 初始化
        {
            dist[entry.first] = std::numeric_limits<int>::max();
        }

        dist[start] = 0;// 加入优先队列

        pq.push({0, start});


        while (!pq.empty())//优先队列不为空
            {
            int u = pq.top().second;// 距离起点最近
            pq.pop();

            if (u == end) break;//如果当前节点是终点，退出循环


            for (const auto &[v, weight]: adj_list_[u])//遍历当前节点的邻居节点
                {

                if (dist[u] + weight < dist[v])//如果当前节点距离更短
                    {

                    dist[v] = dist[u] + weight;//更新

                    prev[v] = u;//前驱节点

                    pq.push({dist[v], v});//加入优先队列
                }
            }
        }

        if (dist[end] == std::numeric_limits<int>::max()) return {};
        std::vector<int> path;

        // 构建最短路径
        // for (int at = end; at != start; at = prev[at]) {
        //     path.push_back(at);
        //     //RCLCPP_INFO_STREAM(this->get_logger(), "at: %d ",at);
        // }
        for (int at = end; at!= start; ) {
            path.push_back(at);
            auto it = prev.find(at);
            if (it == prev.end())// 没有找到前驱节点
                {
                return {};
            }
            at = it->second;
        }

        path.push_back(start);//起点

        std::reverse(path.begin(), path.end());//反转路径

        //RCLCPP_INFO(this->get_logger(), "path: [");
        return path;
    }


    void topic_callback(const judger_interfaces::msg::OverallInfo::SharedPtr msg) {

        //RCLCPP_INFO(this->get_logger(), "topic_callback");
        //int number_of_cities = msg->number_of_cities;

        int number_of_roads = msg->number_of_roads;

        msg->infos.resize(number_of_roads);

        //RCLCPP_INFO(this->get_logger(), "number_of_roads = %d", number_of_roads);

        road_info_.clear();

        for (int i = 0; i < number_of_roads; ++i)//遍历道路信息
            {
            RoadInfo road;
            road.destination=msg->infos[i].destination;
            road.source=msg->infos[i].source;
            road.length=msg->infos[i].length;
            road_info_.push_back(road);
        }

        for (auto &road: road_info_) //初始化图结构
        {
            adj_list_[road.source].push_back({road.destination, road.length});
            adj_list_[road.destination].push_back({road.source, road.length});
        }

        int src_city = msg->src_city;//起点城市


        int des_city = msg->des_city;//终点城市
        //RCLCPP_INFO(this->get_logger(), "src_city: %d", src_city);

        auto path = dijkstra(src_city, des_city);//查找

        // std::vector<int> path;
        // for (int i = 0; i < number_of_roads; ++i) {
        //     path.push_back(msg->infos[i].destination);
        // }

        //rclcpp::Client<judger_interfaces::srv::MyService>::SharedRequest request;

        auto request = std::make_shared<judger_interfaces::srv::MyService::Request>();//发送请求

        request->answer.my_answer.resize(path.size());//初始化拷贝容器

        request->answer.my_answer = path;//拷贝

        // while (!client_->wait_for_service(std::chrono::seconds(1))) {
        //     RCLCPP_INFO(this->get_logger(), "wait_for_service failed");
        // }

        //RCLCPP_INFO(this->get_logger(), "request");

        client_->async_send_request(request,
            std::bind(&TransportationHub::callback, this, std::placeholders::_1));//发送信息
    }

    void callback(const rclcpp::Client<judger_interfaces::srv::MyService>::SharedFuture msg_) {
        //RCLCPP_INFO(this->get_logger(), "topic_callback");
        auto callback_request=msg_.get();//处理srv返回信息
        RCLCPP_INFO(this->get_logger(), "request received %d",callback_request->score);
        RCLCPP_INFO(this->get_logger(), "request sent %s",callback_request->log.c_str());
    }

};

int main(int argc, char **argv) {
    rclcpp::init(argc, argv);

    rclcpp::spin(std::make_shared<TransportationHub>());

    rclcpp::shutdown();

    return 0;
}
