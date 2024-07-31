import 'package:flutter/cupertino.dart';
import 'package:flutter/material.dart';
import 'package:flutter/widgets.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      home: Scaffold(
        backgroundColor: Color.fromARGB(255, 38, 69, 85),
        appBar: AppBar(
          title: const Text("Robotic arm control",
            style: TextStyle(
              color: Color.fromARGB(255, 214, 214, 214),
              fontSize: 28,
              fontWeight: FontWeight.w500
            ),
          ),
          backgroundColor: const Color.fromARGB(255, 77, 114, 133),
          leading: const Icon(Icons.menu),
          actions: const [
            Icon(Icons.bluetooth_rounded)
          ],
        ),
        body: Center(
          child: Container(
            height: 300,
            width: 300,
            decoration: BoxDecoration(
              color: Colors.grey,
              borderRadius: BorderRadius.circular(20)
            ),
            padding: const EdgeInsets.all(25),
            //padding: EdgeInsets.symetric(horizontal: 25, vertical: 50),
            //padding: EdgeInsets.only(left: 25, top: 30),
            /*
            child: const Text("Jorge Sánchez Encinas",
              style: TextStyle(
                color: Colors.black,
                fontSize: 20,
                fontWeight: FontWeight.bold
              ),
            ),*/
            child: const Icon(
              Icons.account_tree_rounded,
              color: Colors.black,
              size: 32,
            ),
          ),
        ),
      ),
    );
  }
}