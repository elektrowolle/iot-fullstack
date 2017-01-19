<template>
  <div id="app">
    <h1>State of drawers</h1>
    <ul>
      <li v-for="drawer in drawers">
        <input class="noFrame" :drawer="drawer.eui" name="name" @input="updateDrawer" :value="drawer.name">
        <input class="noFrame" :drawer="drawer.eui" name="eui" :value="drawer.eui" disabled>
        <input type="range" name="load" :value="drawer.load || 0" min="0" max="1024" disabled>
        <input class="noFrame" :drawer="drawer.eui" name="occupied" @click="updateDrawer" :checked="drawer.occupied == true" type="checkbox">
      </li>
    </ul>
    <label for="new_eui">Name</label>
    <input name="new_name" title="eui" v-model="new_drawer.name" />
    <label for="new_eui">Eui</label>
    <input name="new_eui" title="eui" v-model="new_drawer.eui" />
    <button @click="addDrawer()">Add</button>
  </div>
</template>

<script>
import Store from "./Store.js"
import {mapState} from "vuex"

export default {
  name: 'app',
  store: Store,

  data () {
    return {new_drawer : {}}
  },

  computed:
    mapState({
      drawers: 'drawers',
    }),

  methods:{
    addDrawer(){
      this.$store.dispatch('addDrawer', {
        name: this.new_drawer.name,
        eui: this.new_drawer.eui,
        occupied: false,
        _id: this.new_drawer.eui,
      });
      this.new_drawer = {};
    },

    updateDrawer(e){
      let drawer_id = e.target.attributes.drawer.value;
      let value     = e.target.value;
      console.log(drawer_id);
      this.$store.dispatch('updateDrawer', {
        [e.target.name]: value,
        _id            : drawer_id,
      });
    }
  },
  mounted(){
    setTimeout(()=>this.$store.dispatch('fetchDrawers'),1000);
  }
}
</script>

<style>
.noFrame{
  border:0pt;
}
#app {
  font-family: 'Avenir', Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: #2c3e50;
  margin-top: 60px;
}

h1, h2 {
  font-weight: normal;
}

ul {
  list-style-type: none;
  padding: 0;
}

li {
  /*display: inline-block;*/
  margin: 0 10px;
}

a {
  color: #42b983;
}
</style>
